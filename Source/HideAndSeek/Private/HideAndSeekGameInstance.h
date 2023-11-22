// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "UI/GameOptions.h"
#include "HideAndSeekGameInstance.generated.h"


/**
 * Costum GameInstance to hold the GameOptions between Level Loads.
 */
UCLASS()
class UHideAndSeekGameInstance : public UGameInstance
{
	GENERATED_BODY()

		UHideAndSeekGameInstance();

	GameOptionsPtr Options;

public:
	GameOptionsPtr getOptions();

	


	
};
