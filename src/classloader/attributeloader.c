#include <malloc.h>
#include <classfile.h>
#include <constantparser.h>
#include "attributeloader.h"
#include "primitivereader.h"
#include "utf8utils.h"

int getAttributeType(UTF8* utf8) {
    if(isEqual(utf8, "ConstantValue")) return ATTRIBUTE_CONSTANT_VALUE;
    else if(isEqual(utf8, "Synthetic")) return ATTRIBUTE_SYNTHETIC;
    else if(isEqual(utf8, "Deprecated")) return ATTRIBUTE_DEPRECATED;
    else if(isEqual(utf8, "Code")) return ATTRIBUTE_CODE;
    else if(isEqual(utf8, "LineNumberTable")) return ATTRIBUTE_LINE_NUMBER_TABLE;
    else if(isEqual(utf8, "LocalVariableTable")) return ATTRIBUTE_LOCAL_VARIABLE_TABLE;
    else if(isEqual(utf8, "LocalVariableTypeTable")) return ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE;
    else if(isEqual(utf8, "StackMapTable")) return ATTRIBUTE_STACK_MAP_TABLE;
    else return -1;
}

LineNumberTable* parseLineNumberTable(const uint8_t **content) {
    LineNumberTable* table = malloc(sizeof(LineNumberTable));
    uint16_t size = readuInt16(content);
    table->size = size;

    table->entries = malloc(size * sizeof(LineNumberElement*));;

    for(int i = 0; i < size; i++) {
        LineNumberElement* entry = malloc(sizeof(LineNumberElement));
        entry->startPC = readuInt16(content);
        entry->lineNumber = readuInt16(content);
        table->entries[i] = entry;
    }

    return table;
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
    table->entries = malloc(numEntries * sizeof(StackMapTable));

    for(int i = 0; i < numEntries; i++) {
        StackMapFrame* frame = malloc(sizeof(StackMapFrame));
        uint8_t frameType = readuInt8(content);
        frame->frameType = frameType;
        frame->numLocals = 0;
        frame->stackSize = 0;

        if(frameType < 64) {
            //we do nothing here.
        } else if(frameType < 128) {
            frame->offsetDelta = frameType - 64;
            frame->stackSize = 1;
            frame->stack = malloc(sizeof(VerificationType));
            VerificationType* verificationType = parseVerificationType(content);
            frame->stack[0] = verificationType;
        } else if(frameType < 247) {
            //reserved for future use in the JVM spec.
        } else if(frameType == 247) {
            frame->offsetDelta = readuInt16(content);
            frame->stackSize = 1;
            frame->stack = malloc(sizeof(VerificationType));
            VerificationType* verificationType = parseVerificationType(content);
            frame->stack[0] = verificationType;
        }else if(frameType < 251) {
            frame->offsetDelta = readuInt16(content);
        } else if(frameType < 254) {
            frame->offsetDelta = readuInt16(content);
            frame->numLocals = frameType - 251;
            frame->localVariables = malloc(frame->numLocals * sizeof(VerificationType));
            for(int j = 0; j < frame->numLocals; j++) {
                frame->localVariables[j] = parseVerificationType(content);
            }
        } else if(frameType == 255) {
            frame->offsetDelta = readuInt16(content);
            frame->numLocals = readuInt16(content);
            frame->localVariables = malloc(frame->numLocals * sizeof(VerificationType));
            for(int j = 0; j < frame->numLocals; j++) {
                frame->localVariables[j] = parseVerificationType(content);
            }

            frame->stackSize = readuInt16(content);
            frame->stack = malloc(frame->stackSize * sizeof(VerificationType));
            for(int j = 0; j < frame->stackSize; j++) {
                frame->stack[j] = parseVerificationType(content);
            }
        }

        table->entries[i] = frame;
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
            LineNumberTable* table = parseLineNumberTable(content);
            attribute->tables.lineNumberTable = table;
            break;
        }
        case ATTRIBUTE_LOCAL_VARIABLE_TABLE: {
            LocalVariableTable* table = parseLocalVariableTable(content);
            attribute->tables.localVariableTable = table;
            break;
        }
        case ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE: {
            LocalVariableTypeTable* table = parseLocalVariableTypeTable(content);
            attribute->tables.localVariableTypeTable = table;
            break;
        }
        case ATTRIBUTE_STACK_MAP_TABLE: {
            StackMapTable* table = parseStackMapTable(content);
            attribute->tables.stackMapTable = table;
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
    code->exceptionTable = malloc(sizeof(ExceptionTable));
    code->exceptionTable->length = exceptionTableLength;

    for(int i = 0; i < exceptionTableLength; i++) {
        ExceptionAttributes* attributes = malloc(sizeof(ExceptionAttributes));
        attributes->startpc = readuInt16(content);
        attributes->endpc = readuInt16(content);
        attributes->handlerpc = readuInt16(content);
        attributes->catchType = readuInt16(content);
        code->exceptionTable->exception[i] = attributes;
    }

    uint16_t attributeLength = readuInt16(content);
    code->numAttributes = attributeLength;
    code->attributeInfo = malloc(attributeLength * sizeof(CodeAttributes*));
    for(int i = 0; i < attributeLength; i++) {
        CodeAttributes* attributes = parseCodeAttributes(constantPool, content);
        code->attributeInfo[i] = attributes;
    }
    return code;
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
            default: {
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