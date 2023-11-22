// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AITypes.h"

class AWaypoint;
class AHideAndSeekCharacter;

class GameOptions;
using GameOptionsPtr = TSharedPtr<GameOptions>;

#include "BasicNpcCharacter.generated.h"


/**
* Basic Npc Class with Basic AI Capability for seeking the Player 
*/
UCLASS()
class HIDEANDSEEK_API ABasicNpcCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicNpcCharacter();

	//A Searchlight to show the Player the Field of View and the Viewing Range for Playerdetection.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SearchLight, meta = (AllowPrivateAccess = "true"))
	class USpotLightComponent* SearchLight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when Npc Start Searching the Player, It chooses a random Position around the Player or a random Point around the next Waypoint
	virtual void StartSearching();


	// A list of waypoints that the NPC can traverse
	UPROPERTY(EditInstanceOnly, Category = "Components")
	TArray<AWaypoint*> Waypoints;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//---------------
	// Events for Events in the Gameloop
	void StartNPCEvent(float Time);

	void PlayerCaughtEvent(float Time);

	void PlayerWinEvent();

	//---------------

private: 

	//Referenze to the Player Character
	AHideAndSeekCharacter *Player;

	//Last to the NPC known PlayerPosition, 
	FVector3d LastKnownPlayerPos;

	FAIMoveRequest Request;

	//Position of the Last frame to get Movement Threshold for Navigation.
	FVector3d LastFramePos;

	//float RandomTimer;

	// Index of the current Waypoint
	int WaypointIndex;

	// bool to say if the game is running
	bool IsRunning;

	//Local Reference of the gameoptions for easier Acess
	GameOptionsPtr GameOptions;





};
