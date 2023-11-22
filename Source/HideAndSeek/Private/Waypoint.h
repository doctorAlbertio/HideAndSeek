// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


class ABasicNpcCharacter;


#include "Waypoint.generated.h"

UCLASS()
class AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypoint();

	UPROPERTY(BlueprintReadWrite, Category = Waypoint)
	ABasicNpcCharacter* TargetNpc;

	UPROPERTY(BlueprintReadWrite, Category = Waypoint)
	AWaypoint* NextWaypoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
