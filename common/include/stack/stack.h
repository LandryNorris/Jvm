#ifndef JVM_PROJECT_STACK_H
#define JVM_PROJECT_STACK_H

#include <stdint.h>

typedef struct {
    long capacity;
    int8_t* sp;
    int8_t* values;
} Stack8;

typedef struct {
    long capacity;
    int16_t* sp;
    int16_t* values;
} Stack16;

typedef struct {
    long capacity;
    int32_t* sp;
    int32_t* values;
} Stack32;

typedef struct {
    long capacity;
    int64_t* sp;
    int64_t* values;
} Stack64;

typedef struct {
    long capacity;
    void* sp;
    void* values;
} StackA;

typedef Stack32 Stack;

Stack8* allocStack8(int);

int8_t push(Stack8*, int8_t);

int8_t pop(Stack8*);

int isFull(Stack8*);

int isEmpty(Stack8*);

Stack16* allocStack16(int);

int16_t push16(Stack16*, int16_t);

int16_t pop16(Stack16*);

int isFull16(Stack16*);

int isEmpty16(Stack16*);

Stack32* allocStack32(int);

int32_t push32(Stack32*, int32_t);

int32_t pop32(Stack32*);

/**
 * @param stack
 * @param depth zero-indexed depth to read (0 reads top)
 * @return
 */
int32_t peek32(Stack32* stack, int depth);

int isFull32(Stack32*);

int isEmpty32(Stack32*);

#endif //JVM_PROJECT_STACK_H
