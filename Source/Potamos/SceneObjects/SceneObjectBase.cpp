/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#include "Potamos.h"
#include "SceneObjectBase.h"
#include "SceneObjectGroup.h"

SceneObjectBase::SceneObjectBase() {
	haveAllAssets = false;
}

SceneObjectBase::~SceneObjectBase() {}

void SceneObjectBase::AssetReceived(FGuid id, TArray<uint8_t> data) {}

void SceneObjectBase::FetchAssets() {}
