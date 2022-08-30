#ifndef JVM_PROJECT_EXECUTOR_H
#define JVM_PROJECT_EXECUTOR_H

#include "stackframe.h"

void executeProgram(Program* program, FrameStack* frameStack, ClassFile* classFile);

int executeByNameUtf8(ClassFile* classFile, UTF8* methodName, FrameStack* frameStack);

int executeByName(ClassFile* classFile, char* methodName, FrameStack* frameStack);

#endif //JVM_PROJECT_EXECUTOR_H
