/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#pragma once

#include <inttypes.h>

typedef enum _LLSDType {
	UNDEF,
	LBOOLEAN,
	INTEGER,
	DOUBLE,
	UUID,
	BINARY,
	STRING,
	URI,
	DATE,
	ARRAY,
	MAP
} LLSDType;


/**
 * 
 */
class POTAMOS_API LLSDItem {
public:
	~LLSDItem();
	LLSDItem();

	LLSDType type;
	uint32_t binaryLength;

	union {
		bool booleanData;
		long integerData;
		double doubleData;
		char* stringData;
		uint8_t* binaryData;
	} data;

	TArray<LLSDItem*> arrayData;
	TMap<FString, LLSDItem*> mapData;
};
