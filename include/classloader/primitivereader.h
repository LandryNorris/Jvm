#ifndef PRIMITIVE_READER_H
#define PRIMITIVE_READER_H

#include <stdint.h>

uint8_t readuInt8(const uint8_t** bytes);
uint16_t readuInt16(const uint8_t** bytes);
uint32_t readuInt32(const uint8_t** bytes);
uint64_t readuInt64(const uint8_t** bytes);
float readFloat(const uint8_t** bytes);
double readDouble(const uint8_t** bytes);

#endif
