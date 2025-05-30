#ifndef NATIVE_H
#define NATIVE_H

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

#endif //NATIVE_H
