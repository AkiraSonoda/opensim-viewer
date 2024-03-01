/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "GameFramework/GameMode.h"
#include "MeshActor.h"
#include "PotamosGameMode.generated.h"
class ObjectCreator;

/**
 * 
 */
UCLASS()
class POTAMOS_API APotamosGameMode : public AGameMode {
	GENERATED_BODY()


	APotamosGameMode(const class FObjectInitializer& ObjectInitializer);
	virtual void HandleMatchHasStarted() override;
	virtual void Tick(float deltaSeconds) override;

private:
	TMap<FGuid, AMeshActor*> actors;
	TArray<rapidxml::xml_node<>*> queue;

	void CreateNewActor(FGuid id, TSharedAssetRef data);
	AMeshActor* CreateNewActor(rapidxml::xml_node<>* data, ObjectReadyDelegate d, AMeshActor* act = 0);
	AMeshActor* CreateNewActor(rapidxml::xml_node<>* data);
	void HandleObjectReady(AMeshActor* act);
	ObjectCreator* creator = 0;

	friend class ObjectCreator;
};
