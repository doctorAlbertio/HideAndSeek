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



UCLASS()
class HIDEANDSEEK_API ABasicNpcCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicNpcCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SearchLight, meta = (AllowPrivateAccess = "true"))
	class USpotLightComponent* SearchLight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void StartSearching();

	UPROPERTY(EditInstanceOnly, Category = "Components")
	TArray<AWaypoint*> Waypoints;

	

	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void TimeEvent(float Time);

	void PlayerCaughtEvent(float Time);

	void PlayerWinEvent();
	

private: 
	
		AHideAndSeekCharacter *Player;

		FVector3d LastKnownPlayerPos;

		FAIMoveRequest Request;

		float StartDelay;

		FVector3d LastFramePos;

		float RandomTimer;

		int WaypointIndex;

		bool IsRunning;

		GameOptionsPtr GameOptions;





};
