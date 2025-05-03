#include "classloader/methodparser.h"
#include<stdint.h>
#include <malloc.h>
#include <classloader/primitivereader.h>

#include "classloader/utf8utils.h"

int getArgCount(const char* descriptor) {
    if (descriptor[0] != '(') {
        // Descriptor is not valid
        return -1;
    }

    int numArgs = 0;

    const char* currentCharacter = descriptor+1; // skip the '('

    while (*currentCharacter != '\0' && *currentCharacter != ')') {
        switch (*currentCharacter) {
            case 'B': // byte
            case 'C': // char
            case 'F': // float
            case 'I': // int
            case 'S': // short
            case 'Z': // boolean
                numArgs += 1;
                break;
            case 'J': // long
            case 'D': // double
                numArgs += 2;
                break;
            case 'L': // object types
                numArgs++;
                while (*currentCharacter != '\0' && *currentCharacter != ';') {
                    currentCharacter++;
                }
                break;
            case '[': // array types
                numArgs++;
                currentCharacter++;
                while (*currentCharacter == '[') {
                    currentCharacter++; // skip multiple dimensions
                }
                if (*currentCharacter == 'L') {
                    // skip L
                    while (*currentCharacter != '\0' && *currentCharacter != ';') {
                        currentCharacter++;
                    }
                }
                break;
            default:
                printf("Unexpected string descriptor: %s\n", descriptor);
                return -1; // unknown
        }
        currentCharacter++;
    }

    return numArgs;
}

MethodPool* parseMethodPool(ConstantPool* constantPool, const uint8_t** content) {
    MethodPool* methodPool = malloc(sizeof(MethodPool));

    uint16_t size = readuInt16(content);
    methodPool->size = size;
    methodPool->pool = malloc(size * sizeof(MethodInfo*));

    for(int i = 0; i < size; i++) {
        MethodInfo* methodInfo = malloc(sizeof(MethodInfo));

        methodInfo->accessFlags = readuInt16(content);
        methodInfo->nameIndex = readuInt16(content);
        methodInfo->descriptorIndex = readuInt16(content);

        methodInfo->attributePool = parseAttributes(constantPool, content);
        methodPool->pool[i] = methodInfo;

        UTF8* descriptor = constantPool->pool[methodInfo->descriptorIndex-1]->constant->utf8;
        methodInfo->argumentCount = getArgCount(utf82cstring(descriptor));
    }
    return methodPool;
}
