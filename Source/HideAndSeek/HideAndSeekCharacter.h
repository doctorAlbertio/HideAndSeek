// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include <Components/SpotLightComponent.h>

class GameOptions;
using GameOptionsPtr = TSharedPtr<GameOptions>;


#include "HideAndSeekCharacter.generated.h"



UCLASS(config=Game)
class AHideAndSeekCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//	class USceneComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;
	
	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	AHideAndSeekCharacter();

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeEventDelegate, float Time)
	void TimeEvent(float Time);

	FOnTimeEventDelegate OnTimeEvent;
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateGUIEventDelegate, float Time)
	void UpdateGUIEvent(float DeltaTime);

	FOnUpdateGUIEventDelegate OnUpdateGUIEvent;

	//UPROPERTY(BlueprintCallable)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerCaughtDelegate, float Time)
	void PlayerCaughtEvent(float DeltaTime);

	FOnPlayerCaughtDelegate OnPlayerCaughtEvent;

	DECLARE_MULTICAST_DELEGATE(FOnPlayerWinDelegate)
	void PlayerWinEvent();

	FOnPlayerWinDelegate OnPlayerWinEvent;


	DECLARE_MULTICAST_DELEGATE(FOnStartTimerEventDelegate)
	void StartTimerEvent();

	FOnStartTimerEventDelegate OnStartTimerEvent;

	DECLARE_MULTICAST_DELEGATE(FOnStopTimerEventDelegate)
	void StopTimerEvent();
	FOnStopTimerEventDelegate OnStopTimerEvent;

	// declare overlap begin function
	UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// create trigger capsule
	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Tick(const float DeltaTime) override;
			
	//void Crouch(const FInputActionValue& Value);

	//void onTimeEvent();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	//FORCEINLINE class USceneComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:
	bool IsCountdownRunning;
	float Countdown;

	GameOptionsPtr GameOptions;

	float Playtime;
};

