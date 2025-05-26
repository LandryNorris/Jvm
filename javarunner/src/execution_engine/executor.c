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
#include "memory/primitive_array.h"

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
    return executeByName(executor, executor->loader->mainClass, "main", frameStack, false);
}

int execute(Executor* executor, MethodInfo* method, const ClassFile* classFile, FrameStack* frameStack, bool isVirtual) {
    Code* code = NULL;
    StackFrame* frame;

    for(int i = 0; i < method->attributePool->size; i++) {
        Attribute* attributeInfo = method->attributePool->attributes[i];
        if(attributeInfo->type == ATTRIBUTE_CODE) {

            code = attributeInfo->info->code;
            frame = allocStackFrame(code->maxLocals, code->maxStack, classFile->constantPool);
            StackFrame* lastFrame = peekFrame(frameStack);
            if(lastFrame) {
                int localVariableIndex = 0;
                if (isVirtual) {
                    // the object reference is at the bottom of the stack, but
                    // needs to go at the start of local variables
                    localVariableIndex++;
                }
                for(int j = 0; j < method->argumentCount; j++) {
                    frame->localVariables[localVariableIndex++] = pop32(&lastFrame->operandStack);
                }
                if (isVirtual) {
                    frame->localVariables[0] = pop32(&lastFrame->operandStack);
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

int executeByNameUtf8(Executor* executor, const ClassFile *classFile, UTF8* methodName, UTF8* descriptor, FrameStack* frameStack, bool isVirtual) {
    MethodPool* methodPool = classFile->methodPool;
    for(int i = 0; i < methodPool->size; i++) {
        MethodInfo* info = methodPool->pool[i];
        UTF8* methodNameUtf8 = classFile->constantPool->pool[info->nameIndex-1]->constant->utf8;
        UTF8* descriptorUtf8 = classFile->constantPool->pool[info->descriptorIndex-1]->constant->utf8;

        if(isEqualUtf8(methodNameUtf8, methodName) && isEqualUtf8(descriptor, descriptorUtf8)) {
            return execute(executor, info, classFile, frameStack, isVirtual);
        }
    }
    char* methodNameString = utf82cstring(methodName);
    char* descriptorString = utf82cstring(descriptor);
    printf("Unable to find method with name %s and type %s\n", methodNameString, descriptorString);
    free(methodNameString);
    free(descriptorString);

    return EINVAL;
}

int executeByName(Executor* executor, const ClassFile *classFile, char* methodName, FrameStack* frameStack, bool isVirtual) {
    MethodPool* methodPool = classFile->methodPool;
    for(int i = 0; i < methodPool->size; i++) {
        MethodInfo* info = methodPool->pool[i];
        UTF8* methodNameUtf8 = classFile->constantPool->pool[info->nameIndex-1]->constant->utf8;
        char* currentMethodName = utf82cstring(methodNameUtf8);

        if(strcmp(currentMethodName, methodName) == 0) {
            return execute(executor, info, classFile, frameStack, isVirtual);
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
        printf("Executing: %s\n", instructionNames[instruction]);
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
            case INSTR_ILOAD:
                int8_t index = *++pc;
                int32_t value = locals[index];
                push32(operandStack, value);
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
            case INSTR_ILOAD_3:
                push32(operandStack, locals[3]);
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
            case INSTR_IINC: {
                uint8_t index = *(++pc);
                int8_t constant = (int8_t)*(++pc);
                locals[index] += constant;
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
            case INSTR_ARETURN: // addresses are ints
            case INSTR_IRETURN: {
                StackFrame* lastFrame = getStackFrame(frameStack, 1);
                int value = pop32(operandStack);
                push32(&lastFrame->operandStack, value);
                popFrame(frameStack);
                return; //we are done with this program.
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
            case INSTR_LDC: {
                uint8_t index = *++pc;
                ConstantPoolInfo* constant = classFile->constantPool->pool[index-1];
                switch (constant->tag) {
                    case CONSTANT_INT: {
                        const int value = constant->constant->integer->value;
                        push32(operandStack, value);
                        break;
                    }
                    case CONSTANT_FLOAT: {
                        const float value = constant->constant->f->value;
                        const uint32_t raw = *(uint32_t*)&value;
                        push32(operandStack, (int32_t)raw);
                        break;
                    }
                    case CONSTANT_STRING: {
                        const int index = constant->constant->string->index;
                        UTF8* utf8 = classFile->constantPool->pool[index-1]->constant->utf8;
                        int obj = utf82string(executor->gc, executor->loader, utf8);
                        push32(operandStack, obj);
                        break;
                    }
                }
                break;
            }
            case INSTR_DUP: {
                int top = pop32(operandStack);
                push32(operandStack, top);
                push32(operandStack, top);
                break;
            }
            case INSTR_DUP_X1: {
                // TODO(Landry): Check if both top and next are "category 1 computational type"
                int top = pop32(operandStack);
                int next = pop32(operandStack);
                push32(operandStack, top);
                push32(operandStack, next);
                push32(operandStack, top);
                break;
            }
            case INSTR_DUP_X2: {
                // TODO(Landry): Check for whether value 2 is a category 2 computational type
                //               It will be different?
                int value1 = pop32(operandStack);
                int value2 = pop32(operandStack);
                int value3 = pop32(operandStack);

                push32(operandStack, value1);
                push32(operandStack, value3);
                push32(operandStack, value2);
                push32(operandStack, value1);
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
                NameAndTypeIndex* nameAndTypeIndex = classFile->constantPool->pool[method->nameAndTypeIndex-1]->constant->nameAndTypeIndex;
                uint16_t methodNameIndex = nameAndTypeIndex->nameIndex;
                UTF8* methodName = classFile->constantPool->pool[methodNameIndex-1]->constant->utf8;
                uint16_t otherClassIndex = classFile->constantPool->pool[method->classIndex - 1]->constant->class->nameIndex;
                UTF8* otherClassName = classFile->constantPool->pool[otherClassIndex - 1]->constant->utf8;
                char* otherClassString = utf82cstring(otherClassName);
                UTF8* descriptor = classFile->constantPool->pool[nameAndTypeIndex->descriptorIndex-1]->constant->utf8;

                ClassFile* otherClass = getClassFile(executor->loader, otherClassString);

                executeByNameUtf8(executor, otherClass, methodName, descriptor, frameStack, true);
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
            case INSTR_CASTORE: {
                int value = pop32(operandStack);
                int index = pop32(operandStack);
                int obj = pop32(operandStack);

                setCharArrayValue(executor->gc, obj, index, value);
                break;
            }
            case INSTR_CALOAD: {
                int index = pop32(operandStack);
                int obj = pop32(operandStack);

                int16_t c = (int16_t)getCharArrayValue(executor->gc, obj, index);
                push32(operandStack, c);
                break;
            }
            case INSTR_BASTORE: {
                int value = pop32(operandStack);
                int index = pop32(operandStack);
                int obj = pop32(operandStack);

                setByteArrayValue(executor->gc, obj, index, value);
                break;
            }
            case INSTR_BALOAD: {
                int index = pop32(operandStack);
                int obj = pop32(operandStack);

                int8_t value = (int8_t)getByteArrayValue(executor->gc, obj, index);
                push32(operandStack, value);
                break;
            }
            case INSTR_INVOKESTATIC: {
                int8_t high = *((int8_t*)(++pc));
                int8_t low = *((int8_t*)(++pc));
                int index = high << 8 | low;
                MethodRef* methodRef = classFile->constantPool->pool[index-1]->constant->methodRef;
                //Add getting the ClassFile index later.
                NameAndTypeIndex* nameAndTypeIndex = classFile->constantPool->pool[methodRef->nameAndTypeIndex-1]->constant->nameAndTypeIndex;
                uint16_t methodNameIndex = nameAndTypeIndex->nameIndex;
                UTF8* methodName = classFile->constantPool->pool[methodNameIndex-1]->constant->utf8;
                UTF8* descriptor = classFile->constantPool->pool[nameAndTypeIndex->descriptorIndex-1]->constant->utf8;
                executeByNameUtf8(executor, classFile, methodName, descriptor, frameStack, false);
                break;
            }
            case INSTR_INVOKEVIRTUAL: {
                int8_t high = *((int8_t*)(++pc));
                int8_t low = *((int8_t*)(++pc));
                int index = high << 8 | low;
                MethodRef* methodRef = classFile->constantPool->pool[index-1]->constant->methodRef;

                NameAndTypeIndex* nameAndType = classFile->constantPool->pool[methodRef->nameAndTypeIndex-1]->constant->nameAndTypeIndex;
                uint16_t methodNameIndex = nameAndType->nameIndex;
                UTF8* methodName = classFile->constantPool->pool[methodNameIndex-1]->constant->utf8;

                UTF8* descriptor = classFile->constantPool->pool[nameAndType->descriptorIndex-1]->constant->utf8;

                // TODO(Landry): Test how inherited methods work here
                Class* class = classFile->constantPool->pool[methodRef->classIndex-1]->constant->class;
                UTF8* className = classFile->constantPool->pool[class->nameIndex-1]->constant->utf8;

                if (class->classFile == NULL) {
                    class->classFile = getClassFile(executor->loader, utf82cstring(className));
                }
                executeByNameUtf8(executor, class->classFile, methodName, descriptor, frameStack, true);
                break;
            }
            case INSTR_NEWARRAY: {
                uint8_t type = *++pc;
                int32_t count = pop32(operandStack);

                int obj = createPrimitiveArray(executor->gc, type, count);
                push32(operandStack, obj);
                break;
            }
            case INSTR_ARRAYLENGTH: {
                // TODO(Landry): Handle NullPointerException
                int32_t arrayRef = pop32(operandStack);
                int32_t size = (int32_t) getArrayLength(executor->gc, arrayRef);
                push32(operandStack, size);
                break;
            }

            // If for integers
            case INSTR_IF_ICMPLE: {
                int value2 = pop32(operandStack);
                int value1 = pop32(operandStack);

                int8_t branchByteHigh = *((int8_t*)(++pc));
                int8_t branchByteLow = *((int8_t*)(++pc));

                int16_t branch = branchByteHigh << 8 | branchByteLow;

                if (value1 <= value2) {
                    pc += branch - 3; // we incremented 2 already, and pc++ increments again
                }
                break;
            }

            case INSTR_IF_ICMPEQ: {
                int value2 = pop32(operandStack);
                int value1 = pop32(operandStack);

                int8_t branchByteHigh = *((int8_t*)(++pc));
                int8_t branchByteLow = *((int8_t*)(++pc));
                int16_t branch = branchByteHigh << 8 | branchByteLow;

                if (value1 == value2) {
                    pc += branch - 3; // we incremented 2 already, and pc++ increments again
                }
                break;
            }

            case INSTR_IF_ICMPGE: {
                int value2 = pop32(operandStack);
                int value1 = pop32(operandStack);

                int8_t branchByteHigh = *((int8_t*)(++pc));
                int8_t branchByteLow = *((int8_t*)(++pc));
                int16_t branch = branchByteHigh << 8 | branchByteLow;

                if (value1 >= value2) {
                    pc += branch - 3; // we incremented 2 already, and pc++ increments again
                }
                break;
            }

            case INSTR_IF_ICMPGT: {
                int value2 = pop32(operandStack);
                int value1 = pop32(operandStack);

                int8_t branchByteHigh = *((int8_t*)(++pc));
                int8_t branchByteLow = *((int8_t*)(++pc));
                int16_t branch = branchByteHigh << 8 | branchByteLow;

                if (value1 > value2) {
                    pc += branch - 3; // we incremented 2 already, and pc++ increments again
                }
                break;
            }

            case INSTR_IF_ICMPLT: {
                int value2 = pop32(operandStack);
                int value1 = pop32(operandStack);

                int8_t branchByteHigh = *((int8_t*)(++pc));
                int8_t branchByteLow = *((int8_t*)(++pc));
                int16_t branch = branchByteHigh << 8 | branchByteLow;

                if (value1 < value2) {
                    pc += branch - 3; // we incremented 2 already, and pc++ increments again
                }
                break;
            }

            case INSTR_IF_ICMPNE: {
                int value2 = pop32(operandStack);
                int value1 = pop32(operandStack);

                int8_t branchByteHigh = *((int8_t*)(++pc));
                int8_t branchByteLow = *((int8_t*)(++pc));
                int16_t branch = branchByteHigh << 8 | branchByteLow;

                if (value1 != value2) {
                    pc += branch - 3; // we incremented 2 already, and pc++ increments again
                }
                break;
            }
            case INSTR_GOTO: {
                uint8_t branchByteHigh = *++pc;
                uint8_t branchByteLow = *++pc;
                int16_t branch = branchByteHigh << 8 | branchByteLow;
                pc += branch - 3; // we incremented 2 already, and pc++ increments again
                break;
            }
            case INSTR_I2B: {
                int32_t value = pop32(operandStack);
                int8_t byte = (int8_t) value;
                push32(operandStack, byte);
                break;
            }
            case INSTR_I2C: {
                int32_t value = pop32(operandStack);
                int16_t c = (int16_t) value;
                push32(operandStack, c);
                break;
            }
            case INSTR_I2F: {
                int32_t value = pop32(operandStack);
                float f = (float) value;
                uint32_t fBytes = *(uint32_t*)&f;
                push32(operandStack, (int32_t)fBytes);
                break;
            }

            default:
                printf("Unrecognized instruction %d (%s)\n", instruction, instructionNames[instruction]);
                break;
        }
        pc++;
    }
}
