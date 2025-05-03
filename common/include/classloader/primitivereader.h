#ifndef PRIMITIVE_READER_H
#define PRIMITIVE_READER_H

#include <stdint.h>

uint8_t readuInt8(const uint8_t** bytes);
uint16_t readuInt16(const uint8_t** bytes);
uint32_t readuInt32(const uint8_t** bytes);
uint64_t readuInt64(const uint8_t** bytes);
float readFloat(const uint8_t** bytes);
double readDouble(const uint8_t** bytes);
int8_t readInt8(const uint8_t** bytes);
int16_t readInt16(const uint8_t** bytes);
int32_t readInt32(const uint8_t** bytes);
int64_t readInt64(const uint8_t** bytes);

#endif
