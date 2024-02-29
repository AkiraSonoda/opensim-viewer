/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include "LLSDItem.h"

/**
 * 
 */
class AVINATIONVIEWER_API LLSDDecode {
public:
	LLSDDecode();
	~LLSDDecode();

	LLSDItem* items;

	bool Decode(uint8_t** data);
	void DebugDump();
	static void DumpItem(LLSDItem* item, bool indent = true);
	void Reset();

private:
	LLSDItem* DecodeItem(uint8_t** data);
	double ntohd(double n);
};
