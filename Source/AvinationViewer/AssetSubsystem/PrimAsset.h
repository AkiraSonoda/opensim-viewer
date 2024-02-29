/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "MeshableAsset.h"

/**
 * 
 */
class AVINATIONVIEWER_API PrimAsset : public MeshableAsset {
public:
	PrimAsset();
	virtual ~PrimAsset();

	TArray<uint8_t> xmlData;

private:
	void Process();
};
