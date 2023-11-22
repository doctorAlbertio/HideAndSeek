// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class GameOptions;
using GameOptionsPtr = TSharedPtr<GameOptions>;

/**
 * Basic Cpp Class for Holding the GameOptions in the GameInstance
 */
class GameOptions
{
public:

	//Infos for the npc class;

	// Field of View in Degree
	int NpcFieldOfView;

	// Viewing Range/Radius in Units
	int NpcViewingRange;

	// Percentage Chance that Npc searches Player directly
	int NpcDifficulty;

	//Infos for the player class
	
	
	int CountdownTime;//Countdown Time in Seconds	
	int WinTime;//Win Time in Minutes
	
	//Switch between real Isometric View with Orthografic Camera and a fake Isometric View with perspectivic Camera an 3 Degree auf View
	bool realIsometricView;

	//Give the Game the Posibility to start in the Main Menu.
	bool firstStart;

	//Reset the Options
	void DefaultValues();
	
	GameOptions();
	~GameOptions();
};
