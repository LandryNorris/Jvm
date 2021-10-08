#include <malloc.h>
#include "constantparser.h"
#include "attributeloader.h"
#include "primitivereader.h"
#include "utf8utils.h"

int getAttributeType(UTF8* utf8) {
    if(isEqual(utf8, "ConstantValue")) return ATTRIBUTE_CONSTANT_VALUE;
    if(isEqual(utf8, "Synthetic")) return ATTRIBUTE_SYNTHETIC;
    if(isEqual(utf8, "Deprecated")) return ATTRIBUTE_DEPRECATED;
    if(isEqual(utf8, "Code")) return ATTRIBUTE_CODE;
    if(isEqual(utf8, "LineNumberTable")) return ATTRIBUTE_LINE_NUMBER_TABLE;
    if(isEqual(utf8, "LocalVariableTable")) return ATTRIBUTE_LOCAL_VARIABLE_TABLE;
    if(isEqual(utf8, "LocalVariableTypeTable")) return ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE;
    if(isEqual(utf8, "StackMapTable")) return ATTRIBUTE_STACK_MAP_TABLE;
    if(isEqual(utf8, "BootstrapMethods")) return ATTRIBUTE_BOOTSTRAP_METHODS;
    if(isEqual(utf8, "NestHost")) return ATTRIBUTE_NEST_HOST;
    if(isEqual(utf8, "NestMembers")) return ATTRIBUTE_NEST_MEMBERS;
    else return -1;
}

LineNumberTable* parseLineNumberTable(const uint8_t **content) {
    LineNumberTable* table = malloc(sizeof(LineNumberTable));
    uint16_t size = readuInt16(content);
    table->size = size;
    table->entries = malloc(size * sizeof(LineNumberElement));

    for(int i = 0; i < size; i++) {
        LineNumberElement* entry = &table->entries[i];
        entry->startPC = readuInt16(content);
        entry->lineNumber = readuInt16(content);
    }

    return table;
}

NestHost* parseNestHost(const uint8_t** content) {
    NestHost* result = malloc(sizeof(NestHost));
    result->hostClassIndex = readuInt16(content);
    return result;
}

NestMembers* parseNestMembers(const uint8_t** content) {
    NestMembers* result = malloc(sizeof(NestMembers));

    result->numClasses = readuInt16(content);
    for(int i = 0; i < result->numClasses; i++) {
        result->classes[i] = readuInt16(content);
    }
    return result;
}

LocalVariableTable* parseLocalVariableTable(const uint8_t** contents) {
    LocalVariableTable* table = malloc(sizeof(LocalVariableTable));

    uint16_t localVariableCount = readuInt16(contents);

    table->entries = malloc(localVariableCount * sizeof(LocalVariableElement));

    for(int i = 0; i < localVariableCount; i++) {
        LocalVariableElement* localVar = malloc(sizeof(LocalVariableElement));
        localVar->startPC = readuInt16(contents);
        localVar->length = readuInt16(contents);
        localVar->nameIndex = readuInt16(contents);
        localVar->descriptorIndex = readuInt16(contents);
        localVar->index = readuInt16(contents);
        table->entries[i] = localVar;
    }
    return table;
}

LocalVariableTypeTable* parseLocalVariableTypeTable(const uint8_t** contents) {
    LocalVariableTypeTable* table = malloc(sizeof(LocalVariableTypeTable));

    uint16_t localVariableCount = readuInt16(contents);

    table->entries = malloc(localVariableCount * sizeof(LocalVariableTypeElement));

    for(int i = 0; i < localVariableCount; i++) {
        LocalVariableTypeElement * localVar = malloc(sizeof(LocalVariableTypeElement));
        localVar->startPC = readuInt16(contents);
        localVar->length = readuInt16(contents);
        localVar->nameIndex = readuInt16(contents);
        localVar->signatureIndex = readuInt16(contents);
        localVar->index = readuInt16(contents);
        table->entries[i] = localVar;
    }
    return table;
}

VerificationType* parseVerificationType(const uint8_t** content) {
    VerificationType* verificationType = malloc(sizeof(VerificationType));
    verificationType->tag = readuInt8(content);
    if(verificationType->tag == TYPE_OBJECT || verificationType->tag == TYPE_UNINITIALIZED_VARIABLE) {
        verificationType->index = readuInt16(content);
    } else {
        verificationType->index = 0; //give it a default.
    }
    return verificationType;
}

StackMapTable* parseStackMapTable(const uint8_t** content) {
    StackMapTable* table = malloc(sizeof(StackMapTable));

    uint16_t numEntries = readuInt16(content);
    table->size = numEntries;
    table->entries = malloc(numEntries * sizeof(StackMapFrame));

    for(int i = 0; i < numEntries; i++) {
        StackMapFrame* frame = &table->entries[i];
        uint8_t frameType = readuInt8(content);
        frame->frameType = frameType;
        frame->numLocals = 0;
        frame->stackSize = 0;
        frame->localVariables = NULL;
        frame->stack = NULL;

        if(frameType < 64) {
            //we do nothing here.
            continue;
        } else if(frameType < 128) {
            frame->offsetDelta = frameType - 64;
            frame->stackSize = 1;
            frame->stack = malloc(sizeof(VerificationType*));
            frame->stack[0] = parseVerificationType(content);
        } else if(frameType < 247) {
            //reserved for future use in the JVM spec.
        } else if(frameType == 247) {
            frame->offsetDelta = readuInt16(content);
            frame->stackSize = 1;
            frame->stack = malloc(sizeof(VerificationType*));
            frame->stack[0] = parseVerificationType(content);
        } else if(frameType < 251) {
            frame->offsetDelta = readuInt16(content);
        } else if(frameType < 255) {
            frame->offsetDelta = readuInt16(content);
            frame->numLocals = frameType - 251;
            frame->localVariables = malloc(frame->numLocals * sizeof(VerificationType*));
            for(int j = 0; j < frame->numLocals; j++) {
                frame->localVariables[j] = parseVerificationType(content);
            }
        } else if(frameType == 255) {
            frame->offsetDelta = readuInt16(content);
            frame->numLocals = readuInt16(content);
            frame->localVariables = malloc(frame->numLocals * sizeof(VerificationType*));
            for(int j = 0; j < frame->numLocals; j++) {
                frame->localVariables[j] = parseVerificationType(content);
            }

            frame->stackSize = readuInt16(content);
            frame->stack = malloc(frame->stackSize * sizeof(VerificationType*));
            for(int j = 0; j < frame->stackSize; j++) {
                frame->stack[j] = parseVerificationType(content);
            }
        }
    }
    return table;
}

CodeAttributes* parseCodeAttributes(ConstantPool* constantPool, const uint8_t** content) {
    CodeAttributes* attribute = malloc(sizeof(CodeAttributes));
    attribute->nameIndex = readuInt16(content);

    UTF8* nameUtf8 = constantPool->pool[attribute->nameIndex - 1]->constant->utf8;
    attribute->type = getAttributeType(nameUtf8);

    uint32_t attributeLength = readuInt32(content);
    attribute->attributeLength = attributeLength;

    switch(attribute->type) {
        case ATTRIBUTE_LINE_NUMBER_TABLE: {
            attribute->tables.lineNumberTable = parseLineNumberTable(content);
            break;
        }
        case ATTRIBUTE_LOCAL_VARIABLE_TABLE: {
            attribute->tables.localVariableTable = parseLocalVariableTable(content);
            break;
        }
        case ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE: {
            attribute->tables.localVariableTypeTable = parseLocalVariableTypeTable(content);
            break;
        }
        case ATTRIBUTE_STACK_MAP_TABLE: {
            attribute->tables.stackMapTable = parseStackMapTable(content);
            break;
        }
        default: {
            printf("Unrecognized CodeAttribute type with name index %d and type %d\n", attribute->nameIndex, attribute->type);
            for(int j = 0; j < attributeLength; j++) {
                readuInt8(content); //read the values, but ignore them for now.
            }
            break;
        }
    }
    return attribute;
}

Code* parseCode(ConstantPool* constantPool, const uint8_t** content) {
    Code* code = malloc(sizeof(Code));

    code->maxStack = readuInt16(content);
    code->maxLocals = readuInt16(content);
    uint32_t codeLength = readuInt32(content);

    code->program = malloc(sizeof(Program));
    code->program->length = codeLength;
    code->program->byteCode = malloc(codeLength * sizeof(uint8_t));

    for(int i = 0; i < codeLength; i++) {
        code->program->byteCode[i] = readuInt8(content);
    }

    uint16_t exceptionTableLength = readuInt16(content);
    code->exceptionTable = malloc(sizeof(ExceptionTable) + exceptionTableLength*sizeof(ExceptionAttributes*));
    code->exceptionTable->length = exceptionTableLength;

    for(int i = 0; i < exceptionTableLength; i++) {
        ExceptionAttributes* attributes = &code->exceptionTable->exception[i];
        attributes->startpc = readuInt16(content);
        attributes->endpc = readuInt16(content);
        attributes->handlerpc = readuInt16(content);
        attributes->catchType = readuInt16(content);
    }

    uint16_t attributeLength = readuInt16(content);
    code->numAttributes = attributeLength;
    code->attributeInfo = malloc(attributeLength * sizeof(CodeAttributes*));
    for(int i = 0; i < attributeLength; i++) {
        code->attributeInfo[i] = parseCodeAttributes(constantPool, content);
    }
    return code;
}

BootstrapMethodsList* parseBootstrapMethodsList(const uint8_t** content) {
    uint16_t count = readuInt16(content);
    BootstrapMethodsList* result = malloc(sizeof(BootstrapMethodsList));
    result->count = count;

    result->methods = malloc(count * sizeof(BootstrapMethod));

    for(int i = 0; i < count; i++) {
        result->methods[i].methodRef = readuInt16(content);
        result->methods[i].numArguments = readuInt16(content);

        result->methods[i].arguments = malloc(result->methods[i].numArguments * sizeof(uint16_t));

        for(int j = 0; j < result->methods[i].numArguments; i++) {
            result->methods[i].arguments[j] = readuInt16(content);
        }
    }
    return result;
}

AttributePool* parseAttributes(ConstantPool* constantPool, const uint8_t** content) {
    AttributePool* attributePool = malloc(sizeof(AttributePool));

    uint16_t size = readuInt16(content);
    attributePool->size = size;
    attributePool->attributes = malloc(sizeof(Attribute*) * size);

    for(int i = 0; i < size; i++) {
        Attribute* attribute = malloc(sizeof(Attribute));
        attribute->nameIndex = readuInt16(content);

        UTF8* nameUtf8 = constantPool->pool[attribute->nameIndex - 1]->constant->utf8;
        attribute->type = getAttributeType(nameUtf8);

        uint32_t attributeLength = readuInt32(content);
        attribute->attributeLength = attributeLength;

        switch(attribute->type) {
            case ATTRIBUTE_CONSTANT_VALUE: {
                ConstantValue* constant = malloc(sizeof(ConstantValue));
                constant->valueIndex = readuInt16(content);
                attribute->info = malloc(sizeof(AttributeInfo));
                attribute->info->constantValue = constant;
                break;
            }
            case ATTRIBUTE_DEPRECATED:
            case ATTRIBUTE_SYNTHETIC:
                break; //No members to process, so we just break.
            case ATTRIBUTE_CODE: {
                Code* code = parseCode(constantPool, content);
                attribute->info = malloc(sizeof(AttributeInfo));
                attribute->info->code = code;
                break;
            }
            case ATTRIBUTE_BOOTSTRAP_METHODS: {
                BootstrapMethodsList* bootstrapMethodsList = parseBootstrapMethodsList(content);
                attribute->info = malloc(sizeof(AttributeInfo));
                attribute->info->bootstrapMethods = bootstrapMethodsList;
                break;
            }
            case ATTRIBUTE_NEST_HOST: {
                attribute->info = malloc(sizeof(AttributeInfo));
                attribute->info->nestHost = parseNestHost(content);
            }
            case ATTRIBUTE_NEST_MEMBERS: {
                attribute->info = malloc(sizeof(AttributeInfo));
                attribute->info->nestMembers = parseNestMembers(content);
            }
            default: {
                attribute->info = NULL;
                printf("Unrecognized Attribute type with name index %d and type %d\n", attribute->nameIndex, attribute->type);
                for(int j = 0; j < attributeLength; j++) {
                    readuInt8(content); //read the values, but ignore them for now.
                }
                break;
            }
        }

        attributePool->attributes[i] = attribute;
    }

    return attributePool;
}