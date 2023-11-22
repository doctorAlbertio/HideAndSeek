// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include <AIController.h>

#include "GameFramework/GameModeBase.h"
#include "HideAndSeekGameMode.generated.h"


UCLASS(minimalapi)
class AHideAndSeekGameMode : public AGameModeBase
{
	GENERATED_BODY()



public:
	AHideAndSeekGameMode();
	//TODO entfernen
	//void playCountdown();

	//void addCountdownListener(void(float Time), float Triggertime);

	//AAIController NpcController;
private:

	
};



