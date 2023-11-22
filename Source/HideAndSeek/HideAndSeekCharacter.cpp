// Copyright Epic Games, Inc. All Rights Reserved.

#include "HideAndSeekCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/GameOptions.h"
#include <HideAndSeekGameInstance.h>


//////////////////////////////////////////////////////////////////////////
// AHideAndSeekCharacter

AHideAndSeekCharacter::AHideAndSeekCharacter() :
	IsCountdownRunning(true),
	GameOptions(),
	Countdown(10),
	Playtime(0)
{

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.f, 96.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);



	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom = CreateDefaultSubobject<USceneComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 5.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	CameraBoom->bInheritPitch = false;

	CameraBoom->bDoCollisionTest = false;

	CameraBoom->SetRelativeRotation(FQuat(FVector3d(0, 0, 1), double(PI / 4))
		*FQuat(FVector3d(0, 1, 0), double(PI / 4)/* + LookAxisVector.Y*/));


	
	//TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AUnrealCPPCharacter::OnOverlapEnd);

	//CameraBoom->SetRelativeRotation(FQuat(FVector3d(0, 0, 1), double(PI / 4)/* + LookAxisVector.Y*/));

	//CameraBoom->SetRelativeRotation(FQuat(FVector3d(0, 1, 0), double(PI / 4)/* + LookAxisVector.Y*/));


	//FVector2D(1, 0, 0)
	//FQuat

	

	
	//AddControllerPitchInput(1000);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AHideAndSeekCharacter::TimeEvent(float Time)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("ich stehe im Timeevent"))); }

	if (OnTimeEvent.IsBound()) {
		OnTimeEvent.Broadcast(Time);
	}
}

void AHideAndSeekCharacter::UpdateGUIEvent(float DeltaTime)
{
	if (OnUpdateGUIEvent.IsBound()) {
		//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("ich calle das UpdateGUI Event ")); }
		OnUpdateGUIEvent.Broadcast(DeltaTime);
	}
	else {
		//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString("Das Event ist nicht gebundenS ")); }
	}
}

void AHideAndSeekCharacter::PlayerCaughtEvent(float DeltaTime)
{
	//this->GetController()
	this->DisableInput(Cast<APlayerController>(GetController()));
	if (OnPlayerCaughtEvent.IsBound()) {
		OnPlayerCaughtEvent.Broadcast(DeltaTime);
	}
}

void AHideAndSeekCharacter::PlayerWinEvent()
{
	this->DisableInput(Cast<APlayerController>(GetController()));
	if (OnPlayerWinEvent.IsBound()) {
		OnPlayerWinEvent.Broadcast();
	}
	
}

void AHideAndSeekCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	GameOptions = Cast<UHideAndSeekGameInstance>(GetGameInstance())->getOptions();
	Countdown = GameOptions->CountdownTime;

	if (GameOptions->realIsometricView) {
		FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	}
	else {
		FollowCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
	}


	if (GameOptions->firstStart) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("versuche den npc laufen lassen zu verhindern"), 1)); }

		IsCountdownRunning = false;
		GameOptions->firstStart = false;
	}

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AHideAndSeekCharacter::OnOverlapBegin);

}

//////////////////////////////////////////////////////////////////////////
// Input

void AHideAndSeekCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMyProject9Character::Crouch);
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ACharacter::Crouch);
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACharacter::StopCrouching);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHideAndSeekCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHideAndSeekCharacter::Look);

	}

}

void AHideAndSeekCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 

		//ForwardDirection = (FVector3d)ForwardDirection.RotateAngleAxis(45, FVector3d(0, 0, 1));

		//RightDirection.RotateAngleAxis(45, FVector3d(0, 0, 1));

		AddMovementInput(ForwardDirection.RotateAngleAxis(45, FVector3d(0, 0, 1)), MovementVector.Y);
		AddMovementInput(RightDirection.RotateAngleAxis(45, FVector3d(0,0,1)), MovementVector.X);
	}
}

void AHideAndSeekCharacter::Look(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, Value.Get <FVector2D>());

	//FString string = Value.Get<FVector2f>().ToString();

	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,FString( Value.Get <FVector2D>().ToString())); }
	// input is a Vector2D
	//FVector2D LookAxisVector = Value.Get<FVector2D>(); 
	//FVector2D LookAxisVector = FVector2D(Value.Get<FVector2D>().X, 0);

	
	

	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString(CameraBoom->GetRelativeRotation().ToString())); }
	

	if (Controller != nullptr)
	{


		// add yaw and pitch input to controller
		//AddControllerYawInput(LookAxisVector.X);
		//AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AHideAndSeekCharacter::Tick(const float DeltaTime)
{

	if (Cast<APlayerController>(Controller)->WasInputKeyJustPressed(EKeys::Escape) ||
		Cast<APlayerController>(Controller)->WasInputKeyJustPressed(EKeys::Enter)) {
		PlayerCaughtEvent(DeltaTime);
	}

	[[UNLIKELY]]
	if (Countdown < 0 && IsCountdownRunning && !GameOptions->firstStart) {
		//1 -= DeltaTime;
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Starte mit dem Event den Timer in BasicNpc"), 1)); }
		TimeEvent(DeltaTime);
		IsCountdownRunning = false;
	
	}
	else if(IsCountdownRunning && !GameOptions->firstStart){
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("ich z�hle den countdown runter"), 1)); }
		Countdown -= DeltaTime;
	} 

	if (!IsCountdownRunning && Countdown < 0) {
		if (Playtime < GameOptions->WinTime *60) {
			Playtime += DeltaTime;
			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("ich z�hle die spielzeit hoch: %i"), Playtime)); }

		}
		else {
			//StopTimerEvent();
			PlayerWinEvent();
		}
	}

	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("hier sollte ich das UpdateGUI Event in Tick ansteuern ")); }
	//UpdateGUIEvent(DeltaTime);


}

void AHideAndSeekCharacter::StartTimerEvent()
{
	if (OnStartTimerEvent.IsBound()) {
		OnStartTimerEvent.Broadcast();
	}
}

void AHideAndSeekCharacter::StopTimerEvent()
{
	if (OnStopTimerEvent.IsBound()) {
		OnStopTimerEvent.Broadcast();
	}

}

void AHideAndSeekCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp && !IsCountdownRunning)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));


		PlayerCaughtEvent( 420);
	}
}


/*
void AMyProject9Character::Crouch(const FInputActionValue& Value)
{
	GetCharacterMovement()->bWantsToCrouch = !GetCharacterMovement()->bWantsToCrouch;

}
*/




