/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#include "Potamos.h"
#include "PotBaseActor.h"


// Sets default values
APotBaseActor::APotBaseActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APotBaseActor::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void APotBaseActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}
