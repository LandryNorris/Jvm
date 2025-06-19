#include <stdint.h>

uint8_t readuInt8(const uint8_t** bytes) {
    uint8_t result = (*bytes)[0];
    *bytes += 1;
    return result;
}

int8_t readInt8(const uint8_t** bytes) {
    int8_t result = (*bytes)[0];
    *bytes += 1;
    return result;
}

uint16_t readuInt16(const uint8_t** bytes) {
    uint16_t result = (*bytes)[0] << 8 | (*bytes)[1];
    *bytes += 2;
    return result;
}

int16_t readInt16(const uint8_t** bytes) {
    int16_t result = (*bytes)[0] << 8 | (*bytes)[1];
    *bytes += 2;
    return result;
}

uint32_t readuInt32(const uint8_t** bytes) {
    uint32_t result = (*bytes)[0] << 24 | (*bytes)[1] << 16 | (*bytes)[2] << 8 | (*bytes)[3];
    *bytes += 4;
    return result;
}

int32_t readInt32(const uint8_t** bytes) {
    int32_t result = (*bytes)[0] << 24 | (*bytes)[1] << 16 | (*bytes)[2] << 8 | (*bytes)[3];
    *bytes += 4;
    return result;
}

float readFloat(const uint8_t** bytes) {
    uint32_t result = (*bytes)[0] << 24 | (*bytes)[1] << 16 | (*bytes)[2] << 8 | (*bytes)[3];
    *bytes += 4;
    return *((float*) &result);
}

uint64_t readuInt64(const uint8_t** bytes) {
    uint64_t result = 0;
    for (int i = 0; i < 8; i++) {
        result = (result << 8) | (*bytes)[i];
    }
    *bytes += 8;
    return result;
}

int64_t readInt64(const uint8_t** bytes) {
    int64_t result = 0;
    for (int i = 0; i < 8; i++) {
        result = (result << 8) | (*bytes)[i];
    }
    *bytes += 8;
    return result;
}

double readDouble(const uint8_t** bytes) {
    uint64_t result = 0;
    for (int i = 0; i < 8; i++) {
        result = (result << 8) | (*bytes)[i];
    }
    // printf("Read double bits: %x %x %x %x %x %x %x %x\n", (*bytes)[0], (*bytes)[1], (*bytes)[2],
    // (*bytes)[3], (*bytes)[4], (*bytes)[5], (*bytes)[6], (*bytes)[7]);
    *bytes += 8;
    return *((double*) &result);
}
