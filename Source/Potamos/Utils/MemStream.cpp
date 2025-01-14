/*
 * Copyright (c) Contributors. See LICENSE.TXT & CONTRIBUTORS.TXT
 */
#include "Potamos.h"
#include <string.h>
#include "MemStream.h"


MemStream::MemStream(uint8_t* buffer, size_t l) {
	data = buffer;
	length = l;
	offset = 0;
}

MemStream::~MemStream() {}


size_t MemStream::read(void* buf, size_t len) {
	if (offset + len > length)
		len = length - offset;

	if (len <= 0)
		return 0;

	memcpy(buf, data + offset, len);

	offset += len;

	return len;
}

off_t MemStream::_skip(off_t len) {
	if (len < 0)
		return -1;

	if (offset + len > length)
		len = length - offset;

	offset += len;

	return len;
}

bool MemStream::_seek(off_t pos) {
	if (pos < 0)
		return OPJ_FALSE;

	if (pos > length)
		return OPJ_FALSE;

	offset = pos;

	return OPJ_TRUE;
}
