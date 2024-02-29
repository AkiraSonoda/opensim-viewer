/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include <inttypes.h>

/**
 * 
 */
class AVINATIONVIEWER_API AvinationUtils {
public:
	AvinationUtils();
	~AvinationUtils();

	static float uint16tofloat(uint16_t val, float lower, float upper);
};
