/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "MeshableAsset.h"
#include "J2KDecode.h"

/**
 * 
 */
class POTAMOS_API SculptAsset : public MeshableAsset {
public:
	SculptAsset();
	virtual ~SculptAsset();

	opj_image* image = 0;
	TArray<TArray<FVector>> sculptRows;

private:
	void DecodeImage();
	void Process();
};
