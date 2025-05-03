#include <classloader/attributeloader.h>
#include <interpreter/instructionhelper.h>
#include <interpreter/stackframe.h>
#include <classloader/classfile.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <classloader/utf8utils.h>
#include <stdio.h>
#include <malloc.h>
#include "../../include/execution_engine/executor.h"
#include "memory/memory.h"
#include "utils/constantpoolhelper.h"
#include "memory/objheader.h"

Executor* createExecutor(const char* classPath, const char* mainClassName) {
    Executor* executor = malloc(sizeof(Executor));
    executor->gc = createGarbageCollector();
    executor->loader = createClassLoader(classPath, "java-prebuilt/", mainClassName);

    return executor;
}

void freeExecutor(Executor* executor) {
    freeClassLoader(executor->loader);
    //ToDo: free GC
    free(executor);
}

int runMain(Executor* executor) {
    FrameStack* frameStack = allocFrameStack(100);
    return executeByName(executor, executor->loader->mainClass, "main", frameStack);
}

int execute(Executor* executor, MethodInfo* method, const ClassFile* classFile, FrameStack* frameStack) {
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

    executeProgram(executor, code->program, frameStack, classFile);
    return 0;
}

int executeByNameUtf8(Executor* executor, const ClassFile *classFile, UTF8* methodName, FrameStack* frameStack) {
    MethodPool* methodPool = classFile->methodPool;
    for(int i = 0; i < methodPool->size; i++) {
        MethodInfo* info = methodPool->pool[i];
        UTF8* methodNameUtf8 = classFile->constantPool->pool[info->nameIndex-1]->constant->utf8;

        if(isEqualUtf8(methodNameUtf8, methodName)) {
            return execute(executor, info, classFile, frameStack);
        }
    }
    return EINVAL;
}

int executeByName(Executor* executor, const ClassFile *classFile, char* methodName, FrameStack* frameStack) {
    MethodPool* methodPool = classFile->methodPool;
    for(int i = 0; i < methodPool->size; i++) {
        MethodInfo* info = methodPool->pool[i];
        UTF8* methodNameUtf8 = classFile->constantPool->pool[info->nameIndex-1]->constant->utf8;
        char* currentMethodName = utf82cstring(methodNameUtf8);

        if(strcmp(currentMethodName, methodName) == 0) {
            return execute(executor, info, classFile, frameStack);
        }
    }
    return EINVAL;
}

void executeProgram(Executor* executor, Program* program, FrameStack* frameStack, const ClassFile *classFile) {
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
            case INSTR_IMUL: {
                int b = pop32(operandStack);
                int a = pop32(operandStack);
                push32(operandStack, a*b);
                break;
            }
            case INSTR_IDIV: {
                int b = pop32(operandStack);
                int a = pop32(operandStack);

                // TODO(Landry): Handle 'special case' from idiv specification
                push32(operandStack, a/b);
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
            case INSTR_NEW: {
                uint8_t high = *(++pc);
                uint8_t low = *(++pc);
                int index = high << 8 | low;
                ConstantPoolInfo* constant = classFile->constantPool->pool[index-1];
                char* name = parseClass(constant->constant->class, classFile->constantPool);
                ClassFile* file = getClassFile(executor->loader, name);
                int obj = createObject(executor->gc, file);
                push32(operandStack, obj);
                break;
            }
            case INSTR_DUP: {
                int top = pop32(operandStack);
                push32(operandStack, top);
                push32(operandStack, top);
                break;
            }
            case INSTR_PUTFIELD: {
                uint8_t high = *(++pc);
                uint8_t low = *(++pc);
                int index = high << 8 | low;

                MethodRef* method = classFile->constantPool->pool[index-1]->constant->methodRef;
                NameAndTypeIndex* nameAndType = classFile->constantPool->pool[method->nameAndTypeIndex-1]->constant->nameAndTypeIndex;
                UTF8* nameUtf = classFile->constantPool->pool[nameAndType->nameIndex-1]->constant->utf8;

                char* nameString = utf82cstring(nameUtf);

                uint32_t value = pop32(operandStack);
                uint32_t objRef = pop32(operandStack);

                if(objRef == 0) printf("Object was null. ToDo: Handle this properly");

                ObjHeader* obj = getValue(executor->gc->memoryRegion, (int) objRef);

                setFieldValue32(obj, nameString, value);
                break;
            }
            case INSTR_GETFIELD: {
                uint8_t high = *(++pc);
                uint8_t low = *(++pc);
                int index = high << 8 | low;

                MethodRef* method = classFile->constantPool->pool[index-1]->constant->methodRef;
                NameAndTypeIndex* nameAndType = classFile->constantPool->pool[method->nameAndTypeIndex-1]->constant->nameAndTypeIndex;
                UTF8* nameUtf = classFile->constantPool->pool[nameAndType->nameIndex-1]->constant->utf8;

                char* nameString = utf82cstring(nameUtf);

                uint32_t objRef = pop32(operandStack);

                if(objRef == 0) printf("Object was null. ToDo: Handle this properly");

                ObjHeader* obj = getValue(executor->gc->memoryRegion, (int) objRef);

                int32_t value = getFieldValue32(obj, nameString);
                push32(operandStack, value);
                break;
            }
            case INSTR_INVOKESPECIAL: {
                uint8_t high = *(++pc);
                uint8_t low = *(++pc);
                int index = high << 8 | low;
                ConstantPoolInfo* constant = classFile->constantPool->pool[index-1];
                MethodRef* method = constant->constant->methodRef;
                uint16_t methodNameIndex = classFile->constantPool->pool[method->nameAndTypeIndex-1]->constant->nameAndTypeIndex->nameIndex;
                UTF8* methodName = classFile->constantPool->pool[methodNameIndex-1]->constant->utf8;
                uint16_t otherClassIndex = classFile->constantPool->pool[method->classIndex - 1]->constant->class->nameIndex;
                UTF8* otherClassName = classFile->constantPool->pool[otherClassIndex - 1]->constant->utf8;
                char* otherClassString = utf82cstring(otherClassName);

                ClassFile* otherClass = getClassFile(executor->loader, otherClassString);

                executeByNameUtf8(executor, otherClass, methodName, frameStack);
                break;
            }
            case INSTR_ALOAD_0: {
                int32_t local = locals[0];
                push32(operandStack, local);
                break;
            }
            case INSTR_ALOAD_1: {
                int32_t local = locals[1];
                push32(operandStack, local);
                break;
            }
            case INSTR_ALOAD_2: {
                int32_t local = locals[2];
                push32(operandStack, local);
                break;
            }
            case INSTR_ALOAD_3: {
                int32_t local = locals[3];
                push32(operandStack, local);
                break;
            }
            case INSTR_ASTORE_0: {
                int32_t value = pop32(operandStack);
                locals[0] = value;
                break;
            }
            case INSTR_ASTORE_1: {
                int32_t value = pop32(operandStack);
                locals[1] = value;
                break;
            }
            case INSTR_ASTORE_2: {
                int32_t value = pop32(operandStack);
                locals[2] = value;
                break;
            }
            case INSTR_ASTORE_3: {
                int32_t value = pop32(operandStack);
                locals[3] = value;
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
                executeByNameUtf8(executor, classFile, methodName, frameStack);
                break;
            }
            default:
                printf("Unrecognized instruction %d (%s)\n", instruction, instructionNames[instruction]);
                break;
        }
        pc++;
    }
}
