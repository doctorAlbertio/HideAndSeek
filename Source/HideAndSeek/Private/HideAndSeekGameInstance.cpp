// Fill out your copyright notice in the Description page of Project Settings.


#include "HideAndSeekGameInstance.h"

UHideAndSeekGameInstance::UHideAndSeekGameInstance()
	
{
	//Options = GameOptions();
	//GameOptions::DefaultValues();

	Options = MakeShared<GameOptions>();

	//GameOptions::firstStart = true;
}

GameOptionsPtr UHideAndSeekGameInstance::getOptions()
{
	return Options;
}
