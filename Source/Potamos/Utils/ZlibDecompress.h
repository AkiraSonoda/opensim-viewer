/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include <inttypes.h>

/**
 * 
 */
class POTAMOS_API ZlibDecompress {
public:
	ZlibDecompress();
	~ZlibDecompress();

	static int inflate(const uint8_t* src, uint32_t srcLen, uint8_t** dst);
};
