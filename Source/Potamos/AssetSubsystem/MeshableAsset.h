/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "AssetBase.h"
#include "SubmeshData.h"

/**
 * 
 */
class POTAMOS_API MeshableAsset : public AssetBase {
public:
	MeshableAsset();
	virtual ~MeshableAsset();

	TArray<SubmeshData> submeshes;
};
