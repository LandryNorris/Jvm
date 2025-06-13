#include<stdint.h>
#include <malloc.h>
#include "classloader/fieldpoolloader.h"
#include "classloader/primitivereader.h"
#include "classloader/attributeloader.h"

FieldPool* parseFieldPool(ConstantPool* constantPool, const uint8_t** content) {
    FieldPool* fieldPool = malloc(sizeof(FieldPool));

    uint16_t fieldCount = readuInt16(content);
    fieldPool->size = fieldCount;
    fieldPool->pool = malloc(sizeof(FieldPoolItem*) * fieldCount);

    for(int i = 0; i < fieldCount; i++) {
        FieldPoolItem* fieldPoolItem = malloc(sizeof(FieldPoolItem));
        fieldPoolItem->accessFlags = readuInt16(content);
        const uint16_t nameIndex = readuInt16(content);
        const uint16_t descriptorIndex = readuInt16(content);

        fieldPoolItem->name = constantPool->pool[nameIndex-1]->constant->utf8;
        fieldPoolItem->descriptor = constantPool->pool[descriptorIndex-1]->constant->utf8;
        AttributePool* attributePool = parseAttributes(constantPool, content);

        fieldPoolItem->attributePool = attributePool;
        fieldPool->pool[i] = fieldPoolItem;
    }

    return fieldPool;
}