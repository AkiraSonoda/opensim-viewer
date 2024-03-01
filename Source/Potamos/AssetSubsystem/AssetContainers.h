/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "AssetBase.h"
#include "SharedPointer.h"
#include "Http.h"

DECLARE_DELEGATE_TwoParams(AssetFetchedDelegate, FGuid, TSharedAssetRef)

class AssetCache;

/**
 * 
 */
class POTAMOS_API AssetFetchContainer {
public:
	AssetFetchContainer(FGuid id, TSharedAssetRef a);
	~AssetFetchContainer();

	void AddDispatch(AssetFetchedDelegate d);
	void Dispatch(TArray<AssetFetchedDelegate> d);

	TSharedAssetRef asset;
	FGuid id;

	int cacheHits = 0;

private:
	TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> req;
	int queue;

	TArray<AssetFetchedDelegate> dispatches;

	friend class AssetCache;
};

typedef TSharedRef<AssetFetchContainer, ESPMode::ThreadSafe> TSharedAssetFetchContainerRef;
