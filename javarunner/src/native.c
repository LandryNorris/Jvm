
#include "native.h"

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

#include <ffi.h>
#include <stdlib.h>

#include "classloader/utf8utils.h"

Symbol symbols[100];
int numLoadedSymbols = 0;

Library libraries[5];
int numLoadedLibraries = 0;

Library* loadSharedLibrary(const char* name) {
    // check if already loaded
    for (int i = 0; i < numLoadedLibraries; i++) {
        if (strcmp(libraries[i].name, name) == 0) return &libraries[i];
    }
    libraries[numLoadedLibraries].name = name;
    libraries[numLoadedLibraries].handle = dlopen(name, RTLD_NOW);
    numLoadedLibraries++;
    return &libraries[numLoadedLibraries - 1];
}

Symbol* loadSymbol(const char* name) {
    // check if already loaded
    for (int i = 0; i < numLoadedSymbols; i++) {
        if (strcmp(symbols[i].name, name) == 0) return &symbols[i];
    }

    for (int i = 0; i < numLoadedLibraries; i++) {
        const void* sym = dlsym(libraries[i].handle, name);
        if (sym != nullptr) {
            symbols[numLoadedSymbols].name = name;
            symbols[numLoadedSymbols].sym = sym;
            numLoadedSymbols++;
            return &symbols[numLoadedSymbols-1];
        }
    }

    // TODO(Landry): Handle symbol not found
    printf("Symbol %s not found\n", name);
    return nullptr;
}

void parseDescriptorToFFI(const char* descriptor, ffi_type** types, ffi_type* returnType) {
    // TODO(Landry): Parse Java descriptor to types.
    int typeIndex = 0;
    for (char c = *descriptor; c != '\0'; c = *++descriptor) {
        switch (c) {
            case 'B': {
                types[typeIndex++] = &ffi_type_sint8;
                break;
            }
            case 'C': {
                types[typeIndex++] = &ffi_type_sint8;
                break;
            }
            case 'D': {
                types[typeIndex++] = &ffi_type_double;
                break;
            }
            case 'F': {
                types[typeIndex++] = &ffi_type_float;
                break;
            }
            case 'I': {
                types[typeIndex++] = &ffi_type_sint32;
                break;
            }
            case 'J': {
                types[typeIndex++] = &ffi_type_sint64;
                break;
            }
            case 'S': {
                types[typeIndex++] = &ffi_type_sint16;
                break;
            }
            case 'Z': {
                types[typeIndex++] = &ffi_type_sint32;
                break;
            }
            // TODO(Landry): I think arrays should treated like objects? Maybe primitives are different?
            case '[':
            case 'L': {
                // parse type
                types[typeIndex++] = &ffi_type_uint32;
                char c;
                while ((c = *descriptor++)) {
                    if (c == '\0') {
                        // error, end of string in type name
                        *returnType = ffi_type_void;
                    }
                    if (c == ';') {
                        break;
                    }
                }
                break;
            }
            case ')': {
                // time to parse return type
                break;
            }
        }
    }
    *returnType = ffi_type_void;
}

char* jniName(const ClassFile* classFile, const UTF8* methodName) {
    const Class* clazz = classFile->constantPool->pool[classFile->thisClass-1]->constant->class;
    UTF8* className = classFile->constantPool->pool[clazz->nameIndex-1]->constant->utf8;
    char* classNameString = utf82cstring(className);
    for (int i = 0; i < className->size; i++) {
        if (classNameString[i] == '/') {
            classNameString[i] = '_';
        }
    }

    char* methodNameString = utf82cstring(methodName);

    const char* prefix = "Java_";

    const int length = (int)strlen(prefix) + className->size + 1 + methodName->size;

    char* result = malloc(length + 1);
    snprintf(result, length + 1, "%s%s_%s", prefix, classNameString, methodNameString);

    free(classNameString);
    free(methodNameString);
    return result;
}

void executeNativeMethod(const ClassFile* classFile, const int argc, const UTF8* name, const UTF8* descriptor,
    FrameStack* frameStack, const bool isVirtual) {
    char* jniMethodName = jniName(classFile, name);
    const Symbol* symbolInfo = loadSymbol(jniMethodName);
    free(jniMethodName);

    int argSize = isVirtual ? argc+1 : argc;

    uint32_t args[argSize];

    StackFrame* lastFrame = peekFrame(frameStack);
    if(lastFrame) {
        int localVariableIndex = 0;
        if (isVirtual) {
            // the object reference is at the bottom of the stack, but
            // needs to go at the start of local variables
            localVariableIndex++;
        }
        for(int j = 0; j < argc; j++) {
            args[localVariableIndex++] = pop32(&lastFrame->operandStack);
        }
        if (isVirtual) {
            args[0] = pop32(&lastFrame->operandStack);
        }
    }

    char* descriptorString = utf82cstring(descriptor);
    ffi_type* types[argSize];
    ffi_type returnType;
    if (isVirtual) {
        types[0] = &ffi_type_uint32;
    }
    parseDescriptorToFFI(descriptorString, types+1, &returnType);

    free(descriptorString);

    ffi_cif callInterface;
    const ffi_status status = ffi_prep_cif(&callInterface, FFI_DEFAULT_ABI, argSize, &returnType, types);

    if (status != FFI_OK) {
        printf("Unable to initialize FFI context\n");
        exit(1); // this can be easy to miss, and may cause major issues. Replace with exception later
    }

    void* argPointers[argSize];
    for (int i = 0; i < argSize; i++) {
        argPointers[i] = &args[i];
    }

    uint64_t returnValue = 0;
    ffi_call(&callInterface, (void(*)()) symbolInfo->sym, &returnValue, argPointers);
}
