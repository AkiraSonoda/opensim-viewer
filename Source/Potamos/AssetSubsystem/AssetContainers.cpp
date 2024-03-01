/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#include "Potamos.h"
#include "AssetContainers.h"

AssetFetchContainer::AssetFetchContainer(FGuid in_id, TSharedAssetRef a) : asset(a) {
	id = in_id;
	queue = -1;
	asset->id = in_id;
}

AssetFetchContainer::~AssetFetchContainer() {}

void AssetFetchContainer::AddDispatch(AssetFetchedDelegate d) {
	dispatches.Add(d);
}

void AssetFetchContainer::Dispatch(TArray<AssetFetchedDelegate> d) {
	for (auto it = d.CreateConstIterator(); it; ++it) {
		(*it).ExecuteIfBound(id, asset);
	}
}
