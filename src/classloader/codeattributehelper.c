#include <stdio.h>
#include "codeattributehelper.h"

char* getVerificationTypeString(uint8_t tag) {
    switch(tag) {
        case TYPE_TOP: return "Top";
        case TYPE_INTEGER: return "int";
        case TYPE_LONG: return "long";
        case TYPE_FLOAT: return "float";
        case TYPE_DOUBLE: return "double";
        case TYPE_NULL: return "null";
        case TYPE_OBJECT: return "Object";
        case TYPE_UNINITIALIZED_VARIABLE: return "Uninitialized Variable";
        case TYPE_UNINITIALIZED_THIS: return "Uninitialized This";
    }
    return "Tag Not Valid";
}

void printLineNumberTable(LineNumberTable* table) {
    int size = table->size;

    printf("\tLine Number Table\n");
    for(int i = 0; i < size; i++) {
        LineNumberElement* entry = table->entries[i];
        printf("\t\tline %d: %d\n", entry->lineNumber, entry->startPC);
    }
}

//used by debuggers
void printLocalVariableTable(LocalVariableTable* table) {
    int size = table->size;

    for(int i = 0; i < size; i++) {

    }
}

//used by debuggers
void printLocalVariableTypeTable(LocalVariableTypeTable* table) {
    int size = table->size;

    for(int i = 0; i < size; i++) {

    }
}

void printStackMapTable(StackMapTable* table) {
    int size = table->size;

    printf("\tStackMapTable: number of entries: %d\n", size);
    for(int i = 0; i < size; i++) {
        StackMapFrame* frame = table->entries[i];
        uint8_t frameType = frame->frameType;
        printf("\t\tframeType = %d\n", frameType);

        if(frameType < 64) {
            //we do nothing here.
        } else if(frameType < 128) {
            printf("\t\t\toffsetDelta = %d\n\t\t\tstack = [ %s ]\n", frame->offsetDelta, getVerificationTypeString(frame->stack[0]->tag));
        } else if(frameType < 247) {
            //reserved for future use in the JVM spec.
        } else if(frameType == 247) {
            printf("\t\t\toffsetDelta = %d\n\t\t\tstack = [ %s ]\n", frame->offsetDelta, getVerificationTypeString(frame->stack[0]->tag));
        }else if(frameType < 251) {
            printf("\t\t\toffsetDelta = %d\n", frame->offsetDelta);
        } else if(frameType < 254) {
            uint16_t numLocals = frame->numLocals;

            printf("\t\t\toffsetDelta = %d\n\t\t\tlocals = [ ", frame->offsetDelta);

            for(int j = 0; j < numLocals; j++) {
                printf("%s ", getVerificationTypeString(frame->localVariables[j]->tag));
            }

            printf("]\n");
        } else if(frameType == 255) {
            uint16_t numLocals = frame->numLocals;
            uint16_t stackSize = frame->stackSize;

            printf("\t\t\toffsetDelta = %d\n\t\t\tstack = [ ", frame->offsetDelta);
            for(int j = 0; j < stackSize; j++) {
                printf("%s ", getVerificationTypeString(frame->stack[j]->tag));
            }
            printf("]\n\t\t\tlocals = [ ");
            for(int j = 0; j < numLocals; j++) {
                printf("%s ", getVerificationTypeString(frame->localVariables[j]->tag));
            }
            printf("]\n");
        }
    }
}

void printCodeAttributes(Code* code) {
    uint16_t numAttributes = code->numAttributes;

    for(int i = 0; i < numAttributes; i++) {
        CodeAttributes* attributes = code->attributeInfo[i];

        switch(attributes->type) {
            case ATTRIBUTE_LINE_NUMBER_TABLE:
                printLineNumberTable(attributes->tables.lineNumberTable);
                break;
            case ATTRIBUTE_LOCAL_VARIABLE_TABLE:
                printLocalVariableTable(attributes->tables.localVariableTable);
                break;
            case ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE:
                printLocalVariableTypeTable(attributes->tables.localVariableTypeTable);
                break;
            case ATTRIBUTE_STACK_MAP_TABLE:
                printStackMapTable(attributes->tables.stackMapTable);
        }
    }
}