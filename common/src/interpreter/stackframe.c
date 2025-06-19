#include "interpreter/stackframe.h"

#include <malloc.h>

StackFrame* allocStackFrame(uint16_t localCount, uint16_t stackSize, ConstantPool* constantPool) {
    StackFrame* result = malloc(sizeof(StackFrame) + sizeof(int32_t) * localCount);
    result->operandStack.capacity = stackSize;
    result->operandStack.values = malloc(sizeof(uint32_t) * stackSize);
    result->operandStack.sp = result->operandStack.values - 1;
    result->localVariableCount = localCount;
    result->constantPool = constantPool;
    return result;
}

FrameStack* allocFrameStack(int capacity) {
    FrameStack* result = malloc(sizeof(FrameStack));
    result->capacity = capacity;
    result->values = malloc(capacity * sizeof(StackFrame*));
    result->sp = result->values - 1;
    return result;
}

StackFrame* pushFrame(FrameStack* stack, StackFrame* value) {
    if (isFullFrame(stack)) return NULL;
    *++stack->sp = value;
    return value;
}

StackFrame* popFrame(FrameStack* stack) {
    if (isEmptyFrame(stack)) return NULL;
    return *stack->sp--;
}

StackFrame* peekFrame(FrameStack* stack) {
    if (isEmptyFrame(stack)) return NULL;
    return *stack->sp;
}

int isFullFrame(FrameStack* stack) {
    return stack->sp == stack->values - 1 + stack->capacity;
}

int isEmptyFrame(FrameStack* stack) {
    return stack->sp == stack->values - 1;
}

StackFrame* getStackFrame(FrameStack* stack, int offset) {
    return *(stack->sp - offset);
}

long height(FrameStack* stack) {
    return ((stack->sp) - stack->values);
}
