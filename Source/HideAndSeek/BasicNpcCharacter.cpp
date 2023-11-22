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
//  //DONE Winkel einstellbar machen
//  //DONE Max Seeing Range einbauen evt.
//  //DONE Max Seeing Range einstellbar machen
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


