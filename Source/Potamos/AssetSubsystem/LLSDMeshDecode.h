/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include "LLSDItem.h"

/**
 * 
 */
class POTAMOS_API LLSDMeshDecode {
public:
	LLSDMeshDecode();
	~LLSDMeshDecode();

	static LLSDItem* Decode(uint8_t* mesh, FString lod);
};
