#include <attributeloader.h>
#include <instructionhelper.h>
#include <stackframe.h>
#include <classfile.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <utf8utils.h>
#include <stdio.h>
#include "executor.h"

int execute(MethodInfo* method, ClassFile* classFile, FrameStack* frameStack) {
    Code* code = NULL;
    StackFrame* frame;

    for(int i = 0; i < method->attributePool->size; i++) {
        Attribute* attributeInfo = method->attributePool->attributes[i];
        if(attributeInfo->type == ATTRIBUTE_CODE) {
            code = attributeInfo->info->code;
            frame = allocStackFrame(code->maxLocals, code->maxStack, classFile->constantPool);
            StackFrame* lastFrame = peekFrame(frameStack);
            if(lastFrame) {
                for(int j = 0; j < code->maxLocals; j++) {
                    frame->localVariables[j] = pop32(&lastFrame->operandStack);
                }
            }
            pushFrame(frameStack,frame);
            break;
        }
    }

    if(code == NULL) return EINVAL;

    executeProgram(code->program, frameStack, classFile);
    return 0;
}

int executeByNameUtf8(ClassFile* classFile, UTF8* methodName, FrameStack* frameStack) {
    MethodPool* methodPool = classFile->methodPool;
    for(int i = 0; i < methodPool->size; i++) {
        MethodInfo* info = methodPool->pool[i];
        UTF8* methodNameUtf8 = classFile->constantPool->pool[info->nameIndex-1]->constant->utf8;

        if(isEqualUtf8(methodNameUtf8, methodName)) {
            return execute(info, classFile, frameStack);
        }
    }
    return EINVAL;
}

int executeByName(ClassFile* classFile, char* methodName, FrameStack* frameStack) {
    MethodPool* methodPool = classFile->methodPool;
    for(int i = 0; i < methodPool->size; i++) {
        MethodInfo* info = methodPool->pool[i];
        UTF8* methodNameUtf8 = classFile->constantPool->pool[info->nameIndex-1]->constant->utf8;
        char* currentMethodName = utf82cstring(methodNameUtf8);

        if(strcmp(currentMethodName, methodName) == 0) {
            return execute(info, classFile, frameStack);
        }
    }
    return EINVAL;
}

void executeProgram(Program* program, FrameStack* frameStack, ClassFile* classFile) {
    uint8_t* pc = program->byteCode;
    StackFrame* stackFrame = peekFrame(frameStack);
    Stack32* operandStack = &stackFrame->operandStack;
    int32_t* locals = stackFrame->localVariables;

    while(1) {
        uint8_t instruction = *pc;
        switch(instruction) {
            case INSTR_NOP: break;
            case INSTR_ACONST_NULL:
                push32(operandStack, 0);
                break;
            case INSTR_ICONST_M1:
                push32(operandStack, -1);
                break;
            case INSTR_ICONST_0:
                push32(operandStack, 0);
                break;
            case INSTR_ICONST_1:
                push32(operandStack, 1);
                break;
            case INSTR_ICONST_2:
                push32(operandStack, 2);
                break;
            case INSTR_ICONST_3:
                push32(operandStack, 3);
                break;
            case INSTR_ICONST_4:
                push32(operandStack, 4);
                break;
            case INSTR_ICONST_5:
                push32(operandStack, 5);
                break;
            case INSTR_ILOAD_0:
                push32(operandStack, locals[0]);
                break;
            case INSTR_ILOAD_1:
                push32(operandStack, locals[1]);
                break;
            case INSTR_ILOAD_2:
                push32(operandStack, locals[2]);
                break;
            case INSTR_IADD: {
                int b = pop32(operandStack);
                int a = pop32(operandStack);
                push32(operandStack, a+b);
                break;
            }
            case INSTR_ISUB: {
                int b = pop32(operandStack);
                int a = pop32(operandStack);
                push32(operandStack, a-b);
                break;
            }
            case INSTR_BIPUSH: {
                int8_t value = *((int8_t*)(++pc));
                push32(operandStack, value);
                break;
            }
            case INSTR_SIPUSH: {
                int8_t high = *((int8_t*)(++pc));
                int8_t low = *((int8_t*)(++pc));
                int value = high << 8 | low;
                push32(operandStack, value);
                break;
            }
            case INSTR_ISTORE: {
                uint8_t index = *(++pc);
                int32_t value = pop32(operandStack);
                locals[index] = value;
                break;
            }
            case INSTR_ISTORE_0: {
                int32_t value = pop32(operandStack);
                locals[0] = value;
                break;
            }
            case INSTR_ISTORE_1: {
                int32_t value = pop32(operandStack);
                locals[1] = value;
                break;
            }
            case INSTR_ISTORE_2: {
                int32_t value = pop32(operandStack);
                locals[2] = value;
                break;
            }
            case INSTR_ISTORE_3: {
                int32_t value = pop32(operandStack);
                locals[3] = value;
                break;
            }
            case INSTR_RETURN: {
                popFrame(frameStack);
                return; //we are done with this program.
            }
            case INSTR_IRETURN: {
                StackFrame* lastFrame = getStackFrame(frameStack, 1);
                int value = pop32(operandStack);
                push32(&lastFrame->operandStack, value);
                popFrame(frameStack);
                return; //we are done with this program.
            }
            case INSTR_IF_ICMPGE: {
                uint8_t high = *(++pc);
                uint8_t low = *(++pc);
                int branchOffset = high << 8 | low;
                int value2 = pop32(operandStack);
                int value1 = pop32(operandStack);

                if(value1 >= value2) pc += branchOffset - 3; //we read 2 bytes in this instruction. The pc++ will add another.
                break;
            }
            case INSTR_IF_ICMPGT: {
                uint8_t high = *(++pc);
                uint8_t low = *(++pc);
                int branchOffset = high << 8 | low;
                int value2 = pop32(operandStack);
                int value1 = pop32(operandStack);

                if(value1 > value2) pc += branchOffset - 3; //we read 2 bytes in this instruction. The pc++ will add another.
                break;
            }
            case INSTR_INVOKESTATIC: {
                int8_t high = *((int8_t*)(++pc));
                int8_t low = *((int8_t*)(++pc));
                int index = high << 8 | low;
                MethodRef* methodRef = classFile->constantPool->pool[index-1]->constant->methodRef;
                //Add getting the ClassFile index later.
                uint16_t methodNameIndex = classFile->constantPool->pool[methodRef->nameAndTypeIndex-1]->constant->nameAndTypeIndex->nameIndex;
                UTF8* methodName = classFile->constantPool->pool[methodNameIndex-1]->constant->utf8;
                executeByNameUtf8(classFile, methodName, frameStack);
                break;
            }
            default:
                printf("Unrecognized instruction %d (%s)", instruction, instructionNames[instruction]);
                break;
        }
        pc++;
    }
}