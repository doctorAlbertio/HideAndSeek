// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicNpcCharacter.h"
#include <Kismet/GameplayStatics.h>
#include <HideAndSeek/HideAndSeekCharacter.h>
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "AITypes.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/GameOptions.h"
#include "HideAndSeekCharacter.h"
#include "Waypoint.h"
#include <Engine/SpotLight.h>
#include <HideAndSeekGameInstance.h>

// Sets default values
ABasicNpcCharacter::ABasicNpcCharacter()
	:GameOptions(),
	//StartDelay(30),
	LastFramePos(0, 0, 0),
	//RandomTimer(5.0f),
	WaypointIndex(0),
	IsRunning(false),
	Player()
{

	//StartDelay = 60.0f;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AAIController* con = APawn::GetController<AAIController>();// GetController();

	SearchLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SearchLight"));

	SearchLight->SetupAttachment(RootComponent);

	SearchLight->AddRelativeLocation(FVector(10, 0, 80));

}



// Called when the game starts or when spawned
void ABasicNpcCharacter::BeginPlay()
{
	
	Super::BeginPlay();

	GameOptions = Cast<UHideAndSeekGameInstance>(GetGameInstance())->getOptions();

	//StartDelay = GameOptions->CountdownTime;

	//Setup the Searchlight.

	SearchLight->SetLightFColor(FColor::Green);

	SearchLight->SetOuterConeAngle(GameOptions->NpcFieldOfView / 2);

	SearchLight->SetAttenuationRadius(GameOptions->NpcViewingRange);

	//get the Reference to the player
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHideAndSeekCharacter::StaticClass(), FoundActors);

	Player = Cast<AHideAndSeekCharacter>( FoundActors[0]);

	//create the Request for the IA Movement
	Request = FAIMoveRequest(Player->GetActorLocation());

	//Binding to the Player Events
	Player->OnTimeEvent.AddUObject(this, &ABasicNpcCharacter::StartNPCEvent);

	Player->OnPlayerCaughtEvent.AddUObject(this, &ABasicNpcCharacter::PlayerCaughtEvent);

	Player->OnPlayerWinEvent.AddUObject(this, &ABasicNpcCharacter::PlayerWinEvent);

	//AAIController* con = APawn::GetController<AAIController>();
		
}

void ABasicNpcCharacter::StartSearching()
{
	UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(this->GetWorld());

	FNavLocation newGoal;

	int searchPlayerChance = (100-GameOptions->NpcDifficulty);

	if (!Waypoints.IsEmpty() && FMath::RandRange(0, 100) < searchPlayerChance) {
		navSys->UNavigationSystemV1::GetRandomReachablePointInRadius(Waypoints[WaypointIndex]->GetActorLocation(), 1000, newGoal);

		++WaypointIndex;

		//resetting the Waypoint Array Index
		if (WaypointIndex >= Waypoints.Num()) {
			WaypointIndex = 0;
		}

	} else {
		navSys->UNavigationSystemV1::GetRandomReachablePointInRadius(Player->GetActorLocation(), 1500, newGoal);
	}
		
	LastKnownPlayerPos = newGoal.Location;

} 


// Called every frame
void ABasicNpcCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 if(IsRunning){

		AAIController* con = APawn::GetController<AAIController>();// GetController();

		if (!con->IsFollowingAPath())
		{
			con->MoveToLocation(LastKnownPlayerPos);
		}

		 //TODO Zwischenwert speichern um teueres dividieren im Tick zu verhindern.
		auto FoV = cos(FMath::DegreesToRadians( GameOptions->NpcFieldOfView / 2));

		if (Controller->LineOfSightTo(Player) 
			&& this->GetActorForwardVector().CosineAngle2D(Player->GetActorLocation() - GetActorLocation()) > FoV
			&& FVector::Dist2D( Player->GetActorLocation(), GetActorLocation()) < GameOptions->NpcViewingRange) {

			SearchLight->SetLightFColor(FColor::Red);

			auto pos = Player->GetActorLocation();

			UCharacterMovementComponent* bla = GetCharacterMovement();

			bla->MaxWalkSpeed = 600;

			LastKnownPlayerPos = pos;

			Request.SetCanStrafe(false);

			Request.UpdateGoalLocation(LastKnownPlayerPos);

			con->MoveTo(Request);
		}
		else if (this->GetActorLocation().Distance(this->GetActorLocation(), LastFramePos) < 0.01) {

			StartSearching();

			SearchLight->SetLightFColor(FColor::Blue);
				
			UCharacterMovementComponent* bla = GetCharacterMovement();

			bla->MaxWalkSpeed = 200;

			con->MoveToLocation(LastKnownPlayerPos);
		}
		
		LastFramePos = GetActorLocation();

	}
}

// Called to bind functionality to input
void ABasicNpcCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//TODO Parameter is deprecated and shut not be used
void ABasicNpcCharacter::StartNPCEvent(float Time)
{
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


