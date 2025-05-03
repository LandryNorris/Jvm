#ifndef JVM_PROJECT_STACKFRAME_H
#define JVM_PROJECT_STACKFRAME_H

#include <stack/stack.h>
#include <classloader/constantparser.h>

typedef struct {
    ConstantPool* constantPool;
    Stack32 operandStack;
    uint16_t localVariableCount;
    int32_t localVariables[];
} StackFrame;

typedef struct {
    long capacity;
    StackFrame** sp;
    StackFrame** values;
} FrameStack;

StackFrame* allocStackFrame(uint16_t localCount, uint16_t stackSize, ConstantPool* constantPool);

FrameStack* allocFrameStack(int capacity);

StackFrame* pushFrame(FrameStack*, StackFrame*);

StackFrame* popFrame(FrameStack*);

StackFrame* peekFrame(FrameStack*);

int isFullFrame(FrameStack*);

int isEmptyFrame(FrameStack*);

StackFrame* getStackFrame(FrameStack* stack, int offset);

#endif //JVM_PROJECT_STACKFRAME_H
