/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include "GameFramework/Actor.h"
#include "AvinationBaseActor.generated.h"

UCLASS()
class AVINATIONVIEWER_API AAvinationBaseActor : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAvinationBaseActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
};
