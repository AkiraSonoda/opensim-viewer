/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once
#include "openjpeg.h"

/**
 * 
 */
class POTAMOS_API J2KDecode {
public:
	J2KDecode();
	~J2KDecode();

	bool Decode(TSharedPtr<TArray<uint8_t>, ESPMode::ThreadSafe> data);
	opj_image* image = 0;

private:
};
