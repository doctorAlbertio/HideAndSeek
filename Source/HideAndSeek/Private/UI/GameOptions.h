// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class GameOptions;
using GameOptionsPtr = TSharedPtr<GameOptions>;

/**
 * 
 */
class GameOptions
{
public:

	//Infos for the npc class;
	int NpcFieldOfView;// = 90;
	int NpcViewingRange;// = 2000;
	int NpcDifficulty;// = 10;

	//Infos for the player class
	int CountdownTime;// = 10;
	int WinTime;// = 180;
	bool realIsometricView;// = false;

	bool firstStart;

	void DefaultValues();
	
	GameOptions();
	~GameOptions();
};
