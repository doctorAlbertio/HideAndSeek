// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "UI/GameOptions.h"
#include "HideAndSeekGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class UHideAndSeekGameInstance : public UGameInstance
{
	GENERATED_BODY()

		UHideAndSeekGameInstance();

	//GameOptions* 
	//TSharedPtr<GameOptions>
	GameOptionsPtr Options;

public:
	GameOptionsPtr getOptions();

	


	
};
