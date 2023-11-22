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


UMainUserWidget::UMainUserWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer),
	Textbob("Hide And Seek"),
	CountdownTime(10),
	Controlly(),
	IsGameRunning(true),
	GameOptions()
{

	
	//CountdownTime = GameOptions->CountdownTime;
	
	//MainTextBlock->SetText(FText::FromString(Textbob));
}

//TODO Deprecated
bool UMainUserWidget::Initialize()
{
	
	if (Super::Initialize()) {
		
		//MainTextBlock->SetText(FText::FromString(Textbob));
		
		return true; 
	}

	
	return false;
}

//TODO Deprecated
void UMainUserWidget::PostLoad()
{
	Super::PostLoad();
		//MainTextBlock->SetText(FText::FromString(Textbob));
}

void UMainUserWidget::UpdateGUIEvent(float Deltatime)
{
/*	if (MainTextBlock && IsGameRunning) {
		CountdownTime += Deltatime;

		//FString Minutes = FString("%i:&i",(int)Deltatime,(Deltatime - (int)Deltatime));

		//auto ClockString = FString((int)Deltatime % 60 + ":" + (int)(Deltatime * 100));
		//FTextFormat hi = FTextFormat()
		//FText::Format()

			//FText ClockText = FString(ClockString);//FText(FText::AsNumber((int)Deltatime) FText::FromString(":"))

		auto TimeClock = UKismetStringLibrary::TimeSecondsToString(CountdownTime);

		MainTextBlock->SetText(FText::FromString( TimeClock));

		if (CountdownTime > -5 && CountdownTime < 0) {
			MainTextBlock->SetColorAndOpacity(FSlateColor(FColor::Red));
			//MainTextBlock->Color
		}
		else {
			MainTextBlock->SetColorAndOpacity( FSlateColor(FColor(255, 255, 255, 255)));
		}
		//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("solte im Text die Deltatime setzen ")); }
	}
	else
	{
		//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Ich finde kein Maintextblock ")); }
	}
	*/
}

void UMainUserWidget::Setup()
{
	GameOptions = Cast<UHideAndSeekGameInstance>(GetGameInstance())->getOptions();

	CountdownTime = -GameOptions->CountdownTime;

	MainTextBlock->SetText(FText::FromString(Textbob));

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHideAndSeekCharacter::StaticClass(), FoundActors);

	//ReloadButtonCode->SetVisibility(ESlateVisibility::Collapsed);

	VerticalBoxCode->SetVisibility(ESlateVisibility::Collapsed);

	auto lauf = FoundActors.begin();

	OptionsVerticalBoxCode->SetVisibility(ESlateVisibility::Collapsed);

	FVector3d pos;

	//TODO das anstatt for schleife nutzen überall
	//auto bla = FoundActors[0]


	for (auto i : FoundActors) {
		//pos = i->GetActorLocation();
		auto temp = Cast<AHideAndSeekCharacter>(i);
		temp->OnUpdateGUIEvent.AddUObject(this, &UMainUserWidget::UpdateGUIEvent);

		temp->OnPlayerCaughtEvent.AddUObject(this, &UMainUserWidget::PlayerCaughtEvent);
		Controlly = temp->GetLocalViewingPlayerController();

		temp->OnPlayerWinEvent.AddUObject(this, &UMainUserWidget::PlayerWinEvent);

		temp->OnStartTimerEvent.AddUObject(this, &UMainUserWidget::StartTimerEvent);
	}

	if (GameOptions->firstStart) {
		IsGameRunning = false;
		VerticalBoxCode->SetVisibility(ESlateVisibility::Visible);
		Controlly->SetInputMode(FInputModeUIOnly());
		Controlly->bShowMouseCursor = true;
		//GameOptions->firstStart = false;
	}

	FScriptDelegate test = FScriptDelegate();
	//test.GetUObject()
	test.BindUFunction(this, FName("UMainUserWidget::ReloadLevel"));

	ReloadButtonCode->OnClicked.AddDynamic(this, &UMainUserWidget::ReloadLevel); //(test);//          (this, &UMainUserWidget::ReloadLevel);

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

    // OnUpdateGUIEvent.AddUObject(this, &UMainUserWidget::UpdateGUIEvent);
	
}

void UMainUserWidget::ReloadLevel()
{
	//GetGameInstance()->GetWorld()->GetCurrentLevel().Relo



	UGameplayStatics::OpenLevel(GetGameInstance()->GetWorld(), FName(GetGameInstance()->GetWorld()->GetCurrentLevel()->GetName()));

	Controlly->SetInputMode(FInputModeGameOnly());
	Controlly->bShowMouseCursor = false;

	IsGameRunning = true;

	//ReloadButtonCode->SetVisibility(ESlateVisibility::Collapsed);

	VerticalBoxCode->SetVisibility(ESlateVisibility::Collapsed);

}

void UMainUserWidget::EndGame()
{
	//exit(0);
	//FGenericPlatformMisc::RequestExit(true);
	//GameOptions::firstStart = true;
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

	//OptionsVerticalBoxCode->SetVisibility(! OptionsVerticalBoxCode->GetVisibility());
}

void UMainUserWidget::ChangeFoV(float value)
{
	GameOptions->NpcFieldOfView = FoVSliderCode->GetValue();

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicNpcCharacter::StaticClass(), FoundActors);

	for (auto actor : FoundActors) {
		Cast<ABasicNpcCharacter>(actor)->SearchLight->SetOuterConeAngle(value / 2);
	}

	
	
}

void UMainUserWidget::ChangeViewRange(float value)
{
	GameOptions->NpcViewingRange = ViewRangeSliderCode->GetValue();

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasicNpcCharacter::StaticClass(), FoundActors);

	for (auto actor : FoundActors) {
		Cast<ABasicNpcCharacter>(actor)->SearchLight->SetAttenuationRadius(value);
	}
	
}


void UMainUserWidget::ChangeDifficulty(float value)
{
	GameOptions->NpcDifficulty = DifficultySliderCode->GetValue();

	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("slider hat den value = %i"), DifficultySliderCode->GetValue())); }
	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("gameoptions hat den value = %i"), GameOptions->NpcDifficulty)); }

}

void UMainUserWidget::ChangeCountdownTime(float value)
{
	//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("ändere countdown time mit value = %i"),value)); }
	GameOptions->CountdownTime = CountdownSliderCode->GetValue();
	//CountdownSliderCode->GetValue();

	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("slider hat den value = %i"), CountdownSliderCode->GetValue())); }


	//CountdownSliderCode->Value
	
}

void UMainUserWidget::ChangeWinningTime(float value)
{
	GameOptions->WinTime = value;
	//realIsometricView = bool(true);
}

void UMainUserWidget::PlayerCaughtEvent(float DeltaTime)
{

	//TArray<AActor*> FoundActors;

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyProject9Character::StaticClass(), FoundActors);

	//ReloadButtonCode->SetVisibility(ESlateVisibility::Visible);

	VerticalBoxCode->SetVisibility(ESlateVisibility::Visible);


	//auto bla = Cast<UWidget>(VerticalBoxCode).

	//FWidgetTransform bla = VerticalBoxCode->GetRenderTransform();//.Translation()

	//FWidgetTransform blau = FWidgetTransform()

	//bla.Translation = FVector2D ()

	//VerticalBoxCode->SetRenderTransform()

	//auto lauf = FoundActors.begin();

	IsGameRunning = false;


	//FVector3d pos;

	

	/*for (auto i : FoundActors) {
		//pos = i->GetActorLocation();
		auto temp = Cast<AMyProject9Character>(i);
		//temp->OnUpdateGUIEvent.AddUObject(this, &UMainUserWidget::UpdateGUIEvent);

		//temp->OnPlayerCaughtEvent.AddUObject(this, &UMainUserWidget::PlayerCaughtEvent);
		Controlly = temp->GetLocalViewingPlayerController();
	}
*/
	Controlly->SetInputMode(FInputModeUIOnly());
	Controlly->bShowMouseCursor = true;
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
	//Textbob = FString("You won! Congratulations");
	MainTextBlock->SetText(FText::FromString( TEXT("You won! Congratulations")));
	IsGameRunning = false;
	Controlly->bShowMouseCursor = true;
}

void UMainUserWidget::ChangeCameraMode(bool value)
{
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
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (MainTextBlock && IsGameRunning) {
		CountdownTime += InDeltaTime;

		//FString Minutes = FString("%i:&i",(int)Deltatime,(Deltatime - (int)Deltatime));

		//auto ClockString = FString((int)Deltatime % 60 + ":" + (int)(Deltatime * 100));
		//FTextFormat hi = FTextFormat()
		//FText::Format()

			//FText ClockText = FString(ClockString);//FText(FText::AsNumber((int)Deltatime) FText::FromString(":"))

		auto TimeClock = UKismetStringLibrary::TimeSecondsToString(CountdownTime);

		MainTextBlock->SetText(FText::FromString(TimeClock));

		if (CountdownTime > -5 && CountdownTime < 0) {
			MainTextBlock->SetColorAndOpacity(FSlateColor(FColor::Red));
			//MainTextBlock->Color
		}
		else {
			MainTextBlock->SetColorAndOpacity(FSlateColor(FColor(255, 255, 255, 255)));
		}
		//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("solte im Text die Deltatime setzen ")); }
	}
	else
	{
		//if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString("Ich finde kein Maintextblock ")); }
	}
}




