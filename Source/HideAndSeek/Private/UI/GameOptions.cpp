// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOptions.h"

GameOptions::GameOptions()
	:NpcFieldOfView(90),
	NpcViewingRange(2000),
	NpcDifficulty(20),
	CountdownTime(30),
	WinTime(5),
	realIsometricView(false),
	firstStart(true)
{

}

GameOptions::~GameOptions()
{
}

void GameOptions::DefaultValues()
{
	
	NpcFieldOfView = int(90);
	NpcViewingRange = int(2000);
	NpcDifficulty = int(20);

	CountdownTime = int(30);
	WinTime = int(5);
	realIsometricView = bool(true);
	
	
}
