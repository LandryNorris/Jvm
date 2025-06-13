#include <malloc.h>
#include "stack/stack.h"

Stack8* allocStack8(int capacity) {
    Stack8* result = malloc(sizeof(Stack8));
    result->capacity = capacity;
    result->values = malloc(capacity*sizeof(int8_t));
    result->sp = result->values-1;
    return result;
}

int8_t push(Stack8* stack, int8_t value) {
    if(isFull(stack)) return -1;
    *++stack->sp = value;
    return value;
}

int8_t pop(Stack8* stack) {
    if(isEmpty(stack)) return -1;
    return *stack->sp--;
}

int isFull(Stack8* stack) {
    return stack->sp == stack->values - 1 + stack->capacity;
}

int isEmpty(Stack8* stack) {
    return stack->sp == stack->values-1;
}

Stack16* allocStack16(int capacity) {
    Stack16* result = malloc(sizeof(Stack16));
    result->capacity = capacity;
    result->values = malloc(capacity*sizeof(int16_t));
    result->sp = result->values-1;
    return result;
}

int16_t push16(Stack16* stack, int16_t value) {
    if(isFull16(stack)) return -1;
    *++stack->sp = value;
    return value;
}

int16_t pop16(Stack16* stack) {
    if(isEmpty16(stack)) return -1;
    return *stack->sp--;
}

int isFull16(Stack16* stack) {
    return stack->sp == stack->values - 1 + stack->capacity;
}

int isEmpty16(Stack16* stack) {
    return stack->sp == stack->values-1;
}

Stack32* allocStack32(int capacity) {
    Stack32* result = malloc(sizeof(Stack32));
    result->capacity = capacity;
    result->values = malloc(capacity*sizeof(int32_t));
    result->sp = result->values-1;
    return result;
}

int32_t push32(Stack32* stack, int32_t value) {
    if(isFull32(stack)) {
        return -1;
    }
    *++stack->sp = value;
    return value;
}

int32_t pop32(Stack32* stack) {
    if(isEmpty32(stack)) {
        return -1;
    }
    return *stack->sp--;
}

int32_t peek32(Stack32* stack, int depth) {
    if (stack->sp - depth < stack->values) {
        return -1;
    }
    return *(stack->sp - depth);
}

int isFull32(Stack32* stack) {
    return stack->sp == stack->values - 1 + stack->capacity;
}

int isEmpty32(Stack32* stack) {
    return stack->sp == stack->values-1;
}