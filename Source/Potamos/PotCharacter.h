/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include "GameFramework/Character.h"
#include "PotCharacter.generated.h"

UCLASS()
class POTAMOS_API APotCharacter : public ACharacter {
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APotCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	//handles moving forward/backward
	UFUNCTION()
	void MoveForward(float Val);
	//handles strafing
	UFUNCTION()
	void MoveRight(float Val);
	UFUNCTION()
	void MoveUp(float Value);
	UFUNCTION()
	void TurnAtRate(float Rate);
	UFUNCTION()
	void LookUpAtRate(float Rate);
	UFUNCTION()
	void Quit();

	void ShowFrameRate();

	float BaseTurnRate;
	float BaseLookUpRate;
	bool bAddDefaultMovementBindings;
	int32_t currentKeysFlag;
};
