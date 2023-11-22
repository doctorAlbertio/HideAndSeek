// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOptions.h"

GameOptions::GameOptions()
	:NpcFieldOfView(90),
	NpcViewingRange(2000),
	NpcDifficulty(20),
	CountdownTime(10),
	WinTime(1),
	realIsometricView(false),
	firstStart(true)
{
/*
	NpcFieldOfView = int(90);
	NpcViewingRange = int(2000);
	NpcDifficulty = int(20);

	//CountdownTime in Seconds
	CountdownTime = int(10);
	//WinningTime in Minutes
	WinTime = int(1);
	realIsometricView = bool(false);

	firstStart = true;
	*/
}

GameOptions::~GameOptions()
{
}


//int GameOptions::NpcFieldOfView = int(90);
//int GameOptions::NpcViewingRange = int(2000);
//int GameOptions::NpcDifficulty = int(20);

//int GameOptions::CountdownTime = int(15);
//int GameOptions::WinTime = int(180);
//bool GameOptions::realIsometricView = bool(true);

//bool GameOptions::firstStart = bool(true);

void GameOptions::DefaultValues()
{
	
	NpcFieldOfView = int(90);
	NpcViewingRange = int(2000);
	NpcDifficulty = int(10);

	CountdownTime = int(10);
	WinTime = int(180);
	realIsometricView = bool(true);
	
	
}
