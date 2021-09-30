#include<stdint.h>
#include <malloc.h>
#include "fieldpoolloader.h"
#include "primitivereader.h"
#include "attributeloader.h"

FieldPool* parseFieldPool(ConstantPool* constantPool, const uint8_t** content) {
    FieldPool* fieldPool = malloc(sizeof(FieldPool));

    uint16_t fieldCount = readuInt16(content);
    fieldPool->size = fieldCount;
    fieldPool->pool = malloc(sizeof(FieldPoolItem*) * fieldCount);

    for(int i = 0; i < fieldCount; i++) {
        FieldPoolItem* fieldPoolItem = malloc(sizeof(FieldPoolItem));
        fieldPoolItem->accessFlags = readuInt16(content);
        fieldPoolItem->nameIndex = readuInt16(content);
        fieldPoolItem->descriptorIndex = readuInt16(content);

        AttributePool* attributePool = parseAttributes(constantPool, content);

        fieldPoolItem->attributePool = attributePool;
        fieldPool->pool[i] = fieldPoolItem;
    }

    return fieldPool;
}