#include <stdint.h>

uint8_t readuInt8(const uint8_t** bytes) {
	uint8_t result = (*bytes)[0];
	*bytes += 1;
	return result;
}

uint16_t readuInt16(const uint8_t** bytes) {
	uint16_t result = (*bytes)[0] << 8 | (*bytes)[1];
	*bytes += 2;
	return result;
}

uint32_t readuInt32(const uint8_t** bytes) {
	uint32_t result = (*bytes)[0] << 24 | (*bytes)[1] << 16 | (*bytes)[2] << 8 | (*bytes)[3];
	*bytes += 4;
	return result;
}

float readFloat(const uint8_t** bytes) {
	float result = (*bytes)[0] << 24 | (*bytes)[1] << 16 | (*bytes)[2] << 8 | (*bytes)[3];
	*bytes += 4;
	return result;
}

uint64_t readuInt64(const uint8_t** bytes) {
	uint64_t result = (*bytes)[0] << 56 | (*bytes)[1] << 48 | (*bytes)[2] << 40 | (*bytes)[3] << 32 | (*bytes)[4] << 24 | (*bytes)[5] << 16 | (*bytes)[6] << 8 | (*bytes)[7];
	*bytes += 8;
	return result;
}

double readDouble(const uint8_t** bytes) {
	uint64_t result = (*bytes)[0] << 56 | (*bytes)[1] << 48 | (*bytes)[2] << 40 | (*bytes)[3] << 32 | (*bytes)[4] << 24 | (*bytes)[5] << 16 | (*bytes)[6] << 8 | (*bytes)[7];
	*bytes += 8;
	return result;
}
