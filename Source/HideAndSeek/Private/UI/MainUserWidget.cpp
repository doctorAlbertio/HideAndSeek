// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainUserWidget.h"

#include "Components/TextBlock.h"
#include <HideAndSeek/HideAndSeekCharacter.h>
#include <HideAndSeek/BasicNpcCharacter.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetStringLibrary.h>
#include "Styling/SlateColor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/GameOptions.h"
#include <HideAndSeekGameInstance.h>
#include <Camera/CameraComponent.h>
#include "Components/Button.h"
#include <Components/VerticalBox.h>
#include <Components/Slider.h>
#include <Components/CheckBox.h>


UMainUserWidget::UMainUserWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer),
	CountdownTime(10),
	PlayerController(),
	IsGameRunning(true),
	GameOptions(),
	MainTextBlock()
{
}

void UMainUserWidget::Setup()
{
	GameOptions = Cast<UHideAndSeekGameInstance>(GetGameInstance())->getOptions();

	CountdownTime = -GameOptions->CountdownTime;

	MainTextBlock->SetText(FText::FromString(TEXT("Hide and Seek")));

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHideAndSeekCharacter::StaticClass(), FoundActors);



	//auto lauf = FoundActors.begin();

	OptionsVerticalBoxCode->SetVisibility(ESlateVisibility::Collapsed);

	FVector3d pos;

	if (FoundActors.Num() > 0) {
		auto Player = Cast<AHideAndSeekCharacter>(FoundActors[0]);

		Player->OnPlayerCaughtEvent.AddUObject(this, &UMainUserWidget::PlayerCaughtEvent);

		PlayerController = Player->GetLocalViewingPlayerController();

		Player->OnPlayerWinEvent.AddUObject(this, &UMainUserWidget::PlayerWinEvent);

		Player->OnStartTimerEvent.AddUObject(this, &UMainUserWidget::StartTimerEvent);

	}

	if (GameOptions->firstStart) {
		IsGameRunning = false;
		VerticalBoxCode->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}
	else {
		VerticalBoxCode->SetVisibility(ESlateVisibility::Collapsed);
	}

	FScriptDelegate test = FScriptDelegate();
	test.BindUFunction(this, FName("UMainUserWidget::ReloadLevel"));

	ReloadButtonCode->OnClicked.AddDynamic(this, &UMainUserWidget::ReloadLevel);
	EndGameButtonCode->OnClicked.AddDynamic(this, &UMainUserWidget::EndGame);

	OptionButtonCode->OnClicked.AddDynamic(this, &UMainUserWidget::Options);

	FoVSliderCode->OnValueChanged.AddDynamic(this, &UMainUserWidget::ChangeFoV);
	FoVSliderCode->SetValue(GameOptions->NpcFieldOfView);

	ViewRangeSliderCode->OnValueChanged.AddDynamic(this, &UMainUserWidget::ChangeViewRange);
	ViewRangeSliderCode->SetValue(GameOptions->NpcViewingRange);

	DifficultySliderCode->OnValueChanged.AddDynamic(this, &UMainUserWidget::ChangeDifficulty);
	DifficultySliderCode->SetValue(GameOptions->NpcDifficulty);

	CountdownSliderCode->OnValueChanged.AddDynamic(this, &UMainUserWidget::ChangeCountdownTime);
	CountdownSliderCode->SetValue(GameOptions->CountdownTime);

	WinningTimeSliderCode->OnValueChanged.AddDynamic(this, &UMainUserWidget::ChangeWinningTime);
	WinningTimeSliderCode->SetValue(GameOptions->WinTime);

	IsometricViewModeCheckBoxCode->OnCheckStateChanged.AddDynamic(this, &UMainUserWidget::ChangeCameraMode);
	IsometricViewModeCheckBoxCode->SetCheckedState(ECheckBoxState(GameOptions->realIsometricView));

}

void UMainUserWidget::ReloadLevel()
{

	UGameplayStatics::OpenLevel(GetGameInstance()->GetWorld(), FName(GetGameInstance()->GetWorld()->GetCurrentLevel()->GetName()));

	PlayerController->SetInputMode(FInputModeGameOnly());
	PlayerController->bShowMouseCursor = false;

	IsGameRunning = true;

	VerticalBoxCode->SetVisibility(ESlateVisibility::Collapsed);

}

void UMainUserWidget::EndGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,true);
}

void UMainUserWidget::Options()
{
	if (OptionsVerticalBoxCode->GetVisibility() == ESlateVisibility::Visible) {
		OptionsVerticalBoxCode->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		OptionsVerticalBoxCode->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainUserWidget::ChangeFoV(float value)
{
	GameOptions->NpcFieldOfView = value;

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicNpcCharacter::StaticClass(), FoundActors);

	Cast<ABasicNpcCharacter>(FoundActors[0])->SearchLight->SetOuterConeAngle(value / 2);	
	
}

void UMainUserWidget::ChangeViewRange(float value)
{
	GameOptions->NpcViewingRange = value;

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicNpcCharacter::StaticClass(), FoundActors);

	for (auto actor : FoundActors) {
		Cast<ABasicNpcCharacter>(actor)->SearchLight->SetAttenuationRadius(value);
	}
	
}


void UMainUserWidget::ChangeDifficulty(float value)
{
	GameOptions->NpcDifficulty = value;

	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("slider hat den value = %i"), DifficultySliderCode->GetValue())); }
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("gameoptions hat den value = %i"), GameOptions->NpcDifficulty)); }

}

void UMainUserWidget::ChangeCountdownTime(float value)
{
	GameOptions->CountdownTime = value;
}

void UMainUserWidget::ChangeWinningTime(float value)
{
	GameOptions->WinTime = value;
}

void UMainUserWidget::PlayerCaughtEvent(float DeltaTime)
{

	VerticalBoxCode->SetVisibility(ESlateVisibility::Visible);

	IsGameRunning = false;

	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
}

void UMainUserWidget::StopTimerEvent()
{
	IsGameRunning = false;
}

void UMainUserWidget::StartTimerEvent()
{
	IsGameRunning = true;
}

void UMainUserWidget::PlayerWinEvent()
{
	VerticalBoxCode->SetVisibility(ESlateVisibility::Visible);
	MainTextBlock->SetText(FText::FromString( TEXT("You won! Congratulations")));
	IsGameRunning = false;
	PlayerController->bShowMouseCursor = true;
}

void UMainUserWidget::ChangeCameraMode(bool value)
{
	//TODO die actors halten für die nutzung im button
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHideAndSeekCharacter::StaticClass(), FoundActors);

	TArray<UCameraComponent*> Cameras;

	FoundActors[0]->GetComponents<UCameraComponent>(Cameras);
	if (value) {
		Cameras[0]->SetProjectionMode(ECameraProjectionMode::Orthographic);
		
	}
	else {
		Cameras[0]->SetProjectionMode(ECameraProjectionMode::Perspective);
	}
	
	GameOptions->realIsometricView = value;
}

void UMainUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	if (MainTextBlock && IsGameRunning) {
		CountdownTime += InDeltaTime;

		auto TimeClock = UKismetStringLibrary::TimeSecondsToString(CountdownTime);

		MainTextBlock->SetText(FText::FromString(TimeClock));

		//Turning the timer in the last seconds red
		if (CountdownTime > -5 && CountdownTime < 0) {
			MainTextBlock->SetColorAndOpacity(FSlateColor(FColor::Red));
		}
		else {
			MainTextBlock->SetColorAndOpacity(FSlateColor(FColor(255, 255, 255, 255)));
		}
	}
}




