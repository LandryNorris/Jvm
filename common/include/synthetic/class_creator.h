#ifndef CLASS_CREATOR_H
#define CLASS_CREATOR_H

#include "classloader/classfile.h"

typedef struct {
    const char* name;
    const char* descriptor;
    const int numArgs;
} NativeMethodCreationContext;

typedef struct {
    const char* name;
    int numNativeMethods;
    ClassFile* superclass;
    NativeMethodCreationContext* nativeMethods;
} ClassCreationContext;

/**
 * Create a synthetic class.
 *
 * Notes on memory:
 *  * context.name is not copied, so ownership of the string should be transferred
 *  * nativeMethods[*].name and .descriptor are also not copied
 * @param context
 * @return
 */
ClassFile* createClassFile(const ClassCreationContext* context);

#endif //CLASS_CREATOR_H
