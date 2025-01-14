/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include "inttypes.h"

/**
 * 
 */
class POTAMOS_API TextureEntry {
public:
	TextureEntry();
	//    TextureEntry(TextureEntry& source);
	~TextureEntry();
	static bool Parse(TArray<uint8_t> in, TextureEntry& defaultEntry, TArray<TextureEntry>& entries);
	FLinearColor color;
	float repeatU;
	float repeatV;
	float offsetU;
	float offsetV;
	float rotation;
	float glow;
	uint8_t material;
	uint8_t media;
	FGuid textureId;
	FGuid materialId;

private:
	static bool ReadBitfield(const uint8_t*& pos, uint32_t& faceBits, uint8_t& bitfieldSize);
};
