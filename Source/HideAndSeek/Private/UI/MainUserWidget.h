// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

//TODO Forward Declaration benutzen
#include "Components/Button.h"
#include <Components/VerticalBox.h>


#include <Components/Slider.h>
#include <Components/CheckBox.h>


class UTextBlock;

//Game Options Referenz um das gehen über die GameInstance nicht jedes mal machen zu müssen
class GameOptions;
using GameOptionsPtr = TSharedPtr<GameOptions>;


#include "MainUserWidget.generated.h"



//TODO make this shit right
//using std::sharedPtr<UTextBlock> UTextBlockPtr

/**
 * 
 */
UCLASS()
class UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()

		
public: 

	UMainUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual bool Initialize() override;

	virtual void PostLoad() override;

	//virtual void Tick(float Deltatime) override;

	void UpdateGUIEvent(float Deltatime);

	UFUNCTION(BlueprintCallable)
	void Setup();

	UFUNCTION(BlueprintCallable)
	void ReloadLevel();

	UFUNCTION(BlueprintCallable)
	void EndGame();

	UFUNCTION(BlueprintCallable)
	void Options();

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

	//UFUNCTION(BlueprintCallable)
	void PlayerCaughtEvent(float DeltaTime);

	void StopTimerEvent();

	void StartTimerEvent();

	void PlayerWinEvent();

	//void ChangeCameraMode(bool value);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))

	UPROPERTY(BlueprintReadWrite,Category = UI)

	//TSharedPtr<UTextBlock>
	UTextBlock* MainTextBlock;

	//UPROPERTY(BlueprintReadWrite, Category = UI)
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ReloadButton, meta = (AllowPrivateAccess = "true"))
	//class UButton* ReloadButton;

	FString Textbob;

	float CountdownTime;

	APlayerController* Controlly;

	bool IsGameRunning;

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

	
	GameOptionsPtr GameOptions;

	
	//static friend void Options::DefaultValues();
	/* {
		NpcFieldOfView = int(90);
		NpcViewingRange = int(2000);
		NpcDifficulty = int(10);

		CountdownTime = int(10);
		WinTime = int(180);
		realIsometricView = bool(true);
	}
	*/
};




