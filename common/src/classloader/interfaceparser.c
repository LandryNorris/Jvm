#include "classloader/interfaceparser.h"

#include <stdio.h>
#include <stdlib.h>

#include "classloader/primitivereader.h"

InterfacePool* readInterfacePool(const uint8_t** content) {
    uint16_t size = readuInt16(content);
    InterfacePool* result = malloc(sizeof(InterfacePool) + size * sizeof(uint16_t));

    result->size = size;

    for (int i = 0; i < size; i++) {
        result->interfaces[i] = readuInt16(content);
    }

    return result;
}
