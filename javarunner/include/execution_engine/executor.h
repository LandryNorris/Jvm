#ifndef JVM_PROJECT_EXECUTOR_H
#define JVM_PROJECT_EXECUTOR_H

#include <memory/garbagecollector.h>
#include <classloader/classloader.h>
#include "interpreter/stackframe.h"

typedef struct {
    GarbageCollector* gc;
    ClassLoader* loader;
} Executor;

Executor* createExecutor(const char* classPath, const char* mainClassName);
void executeProgram(Executor* executor, Program* program, FrameStack* frameStack, const ClassFile *classFile);
int runMain(Executor* executor);
int executeByNameUtf8(Executor* executor, const ClassFile *classFile, UTF8* methodName, FrameStack* frameStack);
int executeByName(Executor* executor, const ClassFile *classFile, char* methodName, FrameStack* frameStack);
void freeExecutor(Executor* executor);

#endif //JVM_PROJECT_EXECUTOR_H
