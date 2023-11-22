// Fill out your copyright notice in the Description page of Project Settings.


#include "HideAndSeekGameInstance.h"

UHideAndSeekGameInstance::UHideAndSeekGameInstance()
	
{
	Options = MakeShared<GameOptions>();
}

GameOptionsPtr UHideAndSeekGameInstance::getOptions()
{
	return Options;
}
