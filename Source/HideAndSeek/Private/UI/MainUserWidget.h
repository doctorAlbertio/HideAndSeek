// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


class UButton;
class UVerticalBox;
class USlider;
class UCheckBox;
class UTextBlock;

//Game Options Referenz to get easier Acess to the Options
class GameOptions;
using GameOptionsPtr = TSharedPtr<GameOptions>;


#include "MainUserWidget.generated.h"



//TODO make this shit right
//using std::sharedPtr<UTextBlock> UTextBlockPtr

/**
 * Main Widget Class to Load and Hold the GUI. 
 * Uses a Setup Method to initalise all Members.
 * The Setup Methog get called by the Blueprint
 * after referenzing all UI Element with the Code Equivalent.
 */
UCLASS()
class UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

		
public: 

	UMainUserWidget(const FObjectInitializer& ObjectInitializer);


	//void UpdateGUIEvent(float Deltatime);

	UFUNCTION(BlueprintCallable)
	void Setup();

	UFUNCTION(BlueprintCallable)
	void ReloadLevel();

	UFUNCTION(BlueprintCallable)
	void EndGame();

	UFUNCTION(BlueprintCallable)
	void Options();

	//-----------------------
	//Functions to call when Options Changed
	UFUNCTION(BlueprintCallable)
	void ChangeFoV(float value);

	UFUNCTION(BlueprintCallable)
	void ChangeViewRange(float value);

	UFUNCTION(BlueprintCallable)
	void ChangeDifficulty(float value);

	UFUNCTION(BlueprintCallable)
	void ChangeCountdownTime(float value);

	UFUNCTION(BlueprintCallable)
	void ChangeWinningTime(float value);

	UFUNCTION(BlueprintCallable)
	void ChangeCameraMode(bool value);

	//-----------------------
	//Events Called During the Gameloop
	void PlayerCaughtEvent(float DeltaTime);

	void StopTimerEvent();

	void StartTimerEvent();

	void PlayerWinEvent();
	//----------------------

	//Nativ Tick to Update the Timer Clock
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	//Shows the Name of The Game, The Time or the Winning Message.
	UPROPERTY(BlueprintReadWrite,Category = UI)
	UTextBlock* MainTextBlock;

	//The Time to as Timer in the GUI
	float CountdownTime;

	//The PlayerController vor easier Acess to it.
	APlayerController* PlayerController;

	//Let the tick Method tick.
	bool IsGameRunning;

	//-----------------------
	//The different UI Elements for the Menu
	UPROPERTY(BlueprintReadWrite, Category = UI)
	UButton* ReloadButtonCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UButton* OptionButtonCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UButton* EndGameButtonCode;
	
	UPROPERTY(BlueprintReadWrite, Category = UI)
	UVerticalBox* VerticalBoxCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UVerticalBox* OptionsVerticalBoxCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	USlider* FoVSliderCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	USlider* ViewRangeSliderCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	USlider* DifficultySliderCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	USlider* CountdownSliderCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	USlider* WinningTimeSliderCode;

	UPROPERTY(BlueprintReadWrite, Category = UI)
	UCheckBox* IsometricViewModeCheckBoxCode;

	//------------------------

	//Local Reference of the Gameoptions
	GameOptionsPtr GameOptions;

};




