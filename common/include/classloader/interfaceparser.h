#ifndef INTERFACE_PARSER_H
#define INTERFACE_PARSER_H

#include <stdint.h>

typedef struct {
    uint16_t size;
    uint16_t interfaces[];
} InterfacePool;

InterfacePool* readInterfacePool(const uint8_t** content);

#endif
