#ifndef JVM_PROJECT_TARGETINFO_H
#define JVM_PROJECT_TARGETINFO_H

#include<stdint.h>
typedef struct {
    uint8_t typeParameterIndex;
} TypeParameterTarget;

typedef struct {
    uint16_t superTypeIndex;
} SuperTypeTarget;
#endif
