#ifndef NATIVE_H
#define NATIVE_H

#include "execution_engine/executor.h"
#include "interpreter/stackframe.h"

typedef struct {
    const char* name;
    const void* sym;
} Symbol;

typedef struct {
    const char* name;
    void* handle;
} Library;

Library* loadSharedLibrary(const char* name);
Symbol* loadSymbol(const char* name);
void executeNativeMethod(const ClassFile* classFile, int argc, const UTF8* name,
                         const UTF8* descriptor, FrameStack* frameStack, bool isVirtual);

#endif // NATIVE_H
