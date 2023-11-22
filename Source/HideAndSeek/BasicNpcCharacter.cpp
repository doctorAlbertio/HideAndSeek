// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicNpcCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <HideAndSeek/HideAndSeekCharacter.h>
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "GameFramework/Pawn.h"
//#include "NavigationSystem.h"
//#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
 #include "NavigationSystem.h"
//#include <AIController.h>
#include "GameFramework/Pawn.h"
//#include "Components/InputComponent.h"
//#include "GameFramework/Character.h"
//#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UI/GameOptions.h"


#include "HideAndSeekCharacter.h"
#include "Waypoint.h"
#include <Engine/SpotLight.h>
#include <HideAndSeekGameInstance.h>

//TODO -----CODE------
//	//DONE Rausfinden, wie man die zeitdaten am besten global verarbeitet.
//  //DONE Rausfinden, wie man level reloadet bzw andere lädt
//  //DONE Rausfinden, wie ich die lichtfarbe bei den NPC anpassen kann.
//  //DONE Rausfinden, wie man das Kontakt Event im Code triggern kann.
//  //TODO Einbauen, dass man das Spiel gewinnen kann.
//  //DONE umbauen, das man im Menu anfängt
//  //TODO mit ESC ins Menu gehen,
//TODO -----GUI------
//	//DONE Uhr einbauen 
//  //DONE Maussteuerung in Menu ermöglichen
//  //DONE Menu einbauen
//  //TODO Highscore evt.
//  //DONE Spiel Beenden
// 
// 
// 
//TODO -----KI------
//  //TODO Winkel einstellbar machen
//  //DONE Max Seeing Range einbauen evt.
//  //TODO Max Seeing Range einstellbar machen
//  //TODO Geräuschkonzept einbauen, 
//		//TODO Geräusche durch laufen (evt laufgeschwindigkeit und springen)
//		//TODO Ehm Entfernung zum NPC skaliert mit NPC movement und Interpoliert den Laufweg mehr zu ihm
//  //TODO Mit vergehenden Zeit NPC näher zum Spieler kommen lassen.
//  //TODO Wenn er den kontakt verliert, einmal umgucken lassen
//  //TODO Momentum geben, damit sie nicht auf der stelle drehen
//  //TODO KI kommt näher an dich ran, wenn du dich zu wenig bewegstw
//TODO -----World------
//	//TODO Evt kleiner machen
//  //TODO Evt mehr licht verbauen.
//TODO
//TODO ----Optimizen-----
//	//TODO ConsumeMovementInputVector angucken um KI sauberer zu machen



// Sets default values
ABasicNpcCharacter::ABasicNpcCharacter()
	:GameOptions(),
	StartDelay(30),
	LastFramePos(0, 0, 0),
	RandomTimer(5.0f),
	WaypointIndex(0),
	IsRunning(false),
	Player()
{

	//StartDelay = 60.0f;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//PrimaryActorTick.di

	AAIController* con = APawn::GetController<AAIController>();// GetController();

	//con->

	SearchLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SearchLight"));

	SearchLight->SetupAttachment(RootComponent);

	//SearchLight->AddLocalOffset(FVector(10, 0, 80));

	SearchLight->AddRelativeLocation(FVector(10, 0, 80));

	//SearchLight->SetOuterConeAngle(44);
	

	//SearchLight = CreateDefaultSubobject<ASpotLight>(TEXT("SearchLight"));

	

	//SearchLight->SetupAttachment(RootComponent);
	

	//this->UnPossessed();

	//AIController = AAIController();

	//this->PossessedBy(con);

	//this->ReceivePossessed(AIControllerClass);

}



// Called when the game starts or when spawned
void ABasicNpcCharacter::BeginPlay()
{
	
	Super::BeginPlay();
	//GetActorLocation()

	//SearchLight->SetLightFColor(FColor(0, 255, 0, 255));

	//SearchLight->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform, FName("SearchLight"));

	GameOptions = Cast<UHideAndSeekGameInstance>(GetGameInstance())->getOptions();

	StartDelay = GameOptions->CountdownTime;

	SearchLight->SetLightFColor(FColor::Green);

	SearchLight->SetOuterConeAngle(GameOptions->NpcFieldOfView / 2);

	SearchLight->SetAttenuationRadius(GameOptions->NpcViewingRange);

	//GetCharacterMovement()->RotationRate = FRotator();


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHideAndSeekCharacter::StaticClass(), FoundActors);


	auto lauf = FoundActors.begin();


	FVector3d pos;

	for (auto i : FoundActors) {
		//pos = i->GetActorLocation();
		Player = Cast<AHideAndSeekCharacter>( i);
	}

	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString(pos.ToString())); }

	Request = FAIMoveRequest(Player->GetActorLocation());//FAIMoveRequest(Player);

	Player->OnTimeEvent.AddUObject(this, &ABasicNpcCharacter::TimeEvent);

	Player->OnPlayerCaughtEvent.AddUObject(this, &ABasicNpcCharacter::PlayerCaughtEvent);

	Player->OnPlayerWinEvent.AddUObject(this, &ABasicNpcCharacter::PlayerWinEvent);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicNpcCharacter::OnOverlapBegin);

	AAIController* con = APawn::GetController<AAIController>();



	
}

void ABasicNpcCharacter::StartSearching()
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("Player weg")); }
	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(this->GetWorld());

	FNavLocation newGoal;

	//navSys->UNavigationSystemV1::GetRandomReachablePointInRadius(GetActorLocation(), 500, newGoal);
	
	/*
	for (auto child : Waypoints) {
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue,FString::Printf(TEXT("lol")) + child->GetActorLocation().ToString()); }

		navSys->UNavigationSystemV1::GetRandomReachablePointInRadius(child->GetActorLocation(), 500, newGoal);
	}
	*/

	//int WaypointIndex
	 
	
	int searchPlayerChance = (100-GameOptions->NpcDifficulty);


	//Waypoints[WaypointIndex]->GetActorLocation();
	if (!Waypoints.IsEmpty() && FMath::RandRange(0, 100) < searchPlayerChance) {
		navSys->UNavigationSystemV1::GetRandomReachablePointInRadius(Waypoints[WaypointIndex]->GetActorLocation(), 1000, newGoal);


		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("versuche den waypoint zu erreichen")); }
		//FMath::RandRange(0, 20) 

		++WaypointIndex;

		if (WaypointIndex >= Waypoints.Num()) {
			WaypointIndex = 0;
		}

	} else {
		navSys->UNavigationSystemV1::GetRandomReachablePointInRadius(Player->GetActorLocation(), 1500, newGoal);
	}
		LastKnownPlayerPos = newGoal.Location;

		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("versuche den spieler zu erreichen")); }

		//TArray<AActor*> children;


		

	
	
	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Checking for children"))); }
	//GetAllChildActors(children, true);

	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("children größe %i"),Children.Num())); }


	

} 



void ABasicNpcCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Ich scheine zu treffen"))); }
}

// Called every frame
void ABasicNpcCharacter::Tick(float DeltaTime)
{


	Super::Tick(DeltaTime);
	//[[UNLIKELY]]
	/*if (StartDelay > 0 && IsRunning) {
		StartDelay -= DeltaTime;
		if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("es dauert noch %f Sekunden"),StartDelay)); }
	}
	else*/ if(IsRunning){
				
		
		//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString(pos.ToString())); }

		AAIController* con = APawn::GetController<AAIController>();// GetController();

		if (!con->IsFollowingAPath())
			//con->MoveTo(Request);
		{
			//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("Start KI Movement")); }

			con->MoveToLocation(LastKnownPlayerPos);

			
		}

		//this->GetActorForwardVector().CosineAngle2D(Player->GetActorLocation() - GetActorLocation());


		//TODO die winkel hier mit arcos umwandeln zum testwert
		 
		auto FoV = cos(FMath::DegreesToRadians( GameOptions->NpcFieldOfView / 2));
			// 0.7
		if (Controller->LineOfSightTo(Player) 
			&& this->GetActorForwardVector().CosineAngle2D(Player->GetActorLocation() - GetActorLocation()) > FoV
			&& FVector::Dist2D( Player->GetActorLocation(), GetActorLocation()) < GameOptions->NpcViewingRange) {

			//SearchLight->SetLightFColor(FColor(255, 0, 0, 255));
			SearchLight->SetLightFColor(FColor::Red);

			auto pos = Player->GetActorLocation();

			UCharacterMovementComponent* bla = GetCharacterMovement();

			bla->MaxWalkSpeed = 600;

			LastKnownPlayerPos = pos;

			Request.SetCanStrafe(false);

			Request.UpdateGoalLocation(LastKnownPlayerPos);

			con->MoveTo(Request);

			if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("Sehe den Player")); }




		//	else {
				//Request.UpdateGoalLocation(pos);
		//	}






			//FNavPathSharedPtr OutPath;

			//AAIController::Moves(Request, OutPath);

			//AIController.MoveTo();

			//((AAIController)Controller).MoveTo();



			//FVector2D MovementVector = FVector2D(pos.X, pos.Y);


				// find out which way is forward
				//const FRotator Rotation = GetActorRotation();//Controller->GetControlRotation();



			//	auto pos2 = this->GetActorLocation();



				//const FRotator Rotation = FVector3d(pos.X - pos2.X, pos.Y - pos2.Y, pos.Z - pos2.Z).Rotation();
				//const FRotator YawRotation(0, Rotation.Yaw, 0);

			//auto runVec = FVector3d(pos.X - pos2.X, pos.Y - pos2.Y, pos.Z - pos2.Z);

			//runVec.Normalize();

			//AddMovementInput(runVec, 0.5);

		}
		else {
			if (GEngine) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString("Suche Player")); 
				//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Abstand ist %f"),
				//	this->GetActorLocation().DistXY(this->GetActorLocation(), LastKnownPlayerPos))); }
			}

			//if (this->GetActorLocation().DistXY(this->GetActorLocation(), FVector3d(LastKnownPlayerPos.X, LastKnownPlayerPos.Y, 0)) < 150) {
			if (this->GetActorLocation().Distance(this->GetActorLocation(), LastFramePos) < 0.01) {

				StartSearching();

				SearchLight->SetLightFColor(FColor::Blue);
				
				UCharacterMovementComponent* bla = GetCharacterMovement();

				bla->MaxWalkSpeed = 200;

				
				con->MoveToLocation(LastKnownPlayerPos);
			}
			//con->StopMovement();
		}

		// get forward vector
		//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		//const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		//AddMovementInput(ForwardDirection, MovementVector.Y);
		//AddMovementInput(RightDirection, MovementVector.X);

		LastFramePos = GetActorLocation();

	}
}

// Called to bind functionality to input
void ABasicNpcCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasicNpcCharacter::TimeEvent(float Time)
{
	
	//PrimaryActorTick.bCanEverTick = true;
	IsRunning = true;
}

void ABasicNpcCharacter::PlayerCaughtEvent(float Time)
{
	GetController()->StopMovement();
	IsRunning = false;
		
		
}

void ABasicNpcCharacter::PlayerWinEvent()
{
	GetController()->StopMovement();
	IsRunning = false;
}


