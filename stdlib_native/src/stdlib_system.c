#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../javarunner/include/execution_engine/executor.h"
#include "classloader/classloader.h"
#include "memory/objheader.h"
#include "memory/primitive_array.h"
#include "synthetic/class_creator.h"
#include "utils/log.h"

void Java_java_lang_Printer_print() {
    verboseLog("System is printing!\n");
}

void Java_java_lang_SyntheticPrinter_print(uint32_t obj, uint32_t text) {
    Executor* executor = getMainExecutor();
    ObjHeader* stringObj = getValue(executor->gc->memoryRegion, (int) text);

    // For Strings, the value is fields[0]
    int valueOffset = stringObj->fields[0]->offset;
    int valueRef = 0;
    memcpy(&valueRef, &stringObj->data[valueOffset], sizeof(int));

    const PrimitiveArray* arrayHeader = getValue(executor->gc->memoryRegion, valueRef);

    fwrite(arrayHeader->memory, sizeof(char), arrayHeader->length, stdout);
    fflush(stdout);
}

void Java_java_lang_System_setupPrinter() {
    const Executor* executor = getMainExecutor();

    uint8_t loadedFresh;
    ClassFile* printStreamClass =
        getClassFile(executor->loader, "java/io/PrintStream", &loadedFresh);

    const NativeMethodCreationContext nativeMethods[] = {
        {
            .name = "print",
            .descriptor = "(Ljava/lang/String;)V",
            .numArgs = 1,
        },
    };

    const ClassCreationContext context = {
        .name = "java/lang/SyntheticPrinter",
        .numNativeMethods = 1,
        .superclass = printStreamClass,
        .nativeMethods = nativeMethods,
    };
    ClassFile* syntheticPrinter = createClassFile(&context);

    addClass(executor->loader->classPool, syntheticPrinter);

    ClassFile* system = getClassFile(executor->loader, "java/lang/System", &loadedFresh);

    const int printerObject = createObject(executor->gc, syntheticPrinter);

    setInt32StaticField(system, "out", printerObject);
    verboseLog("Found system\n");
}

double Java_java_lang_Math_acos(double a) {
    return acos(a);
}
double Java_java_lang_Math_asin(double a) {
    return asin(a);
}
double Java_java_lang_Math_atan(double a) {
    return atan(a);
}
double Java_java_lang_Math_atan2(double a, double b) {
    return atan2(a, b);
}
double Java_java_lang_Math_cbrt(double a) {
    return cbrt(a);
}
double Java_java_lang_Math_ceil(double a) {
    return ceil(a);
}
double Java_java_lang_Math_cos(double a) {
    return cos(a);
}
double Java_java_lang_Math_cosh(double a) {
    return cosh(a);
}
double Java_java_lang_Math_exp(double a) {
    return exp(a);
}
double Java_java_lang_Math_expm1(double a) {
    return expm1(a);
}
double Java_java_lang_Math_floor(double a) {
    return floor(a);
}
double Java_java_lang_Math_floorDiv(int a, int b) {
    // TODO(Landry): Handle properly?
    return floor(a/b);
}
double Java_java_lang_Math_sqrt(double a) {
    return sqrt(a);
}
double Java_java_lang_Math_log(double a) {
    return log(a);
}
double Java_java_lang_Math_log10(double a) {
    return log10(a);
}
double Java_java_lang_Math_log1p(double a) {
    return log1p(a);
}
double Java_java_lang_Math_random() {
    return (double)random() / (double) RAND_MAX;
}

int Java_java_io_File_openFile(uint32_t obj, uint32_t pathIndex) {
    Executor* executor = getMainExecutor();
    ObjHeader* stringObj = getValue(executor->gc->memoryRegion, (int) pathIndex);

    // For Strings, the value is fields[0]
    int valueOffset = stringObj->fields[0]->offset;
    int valueRef = 0;
    memcpy(&valueRef, &stringObj->data[valueOffset], sizeof(int));

    const PrimitiveArray* arrayHeader = getValue(executor->gc->memoryRegion, valueRef);
    char* path = malloc(arrayHeader->length+1); // remember null terminator
    memcpy(path, arrayHeader->memory, arrayHeader->length);
    path[arrayHeader->length] = 0;

    FILE* fd = fopen(path, "a+");
    if (fd == nullptr) {
        traceLog("Failed to  open file at: %s, %d\n", path, errno);
        free(path);
        return 0;
    }
    free(path);
    return fileno(fd);
}

int Java_java_io_File_getPermissions(int fd) {

}

int Java_java_io_File_existsInternal(uint32_t obj, uint32_t pathIndex) {
    Executor* executor = getMainExecutor();
    ObjHeader* stringObj = getValue(executor->gc->memoryRegion, (int) pathIndex);

    // For Strings, the value is fields[0]
    int valueOffset = stringObj->fields[0]->offset;
    int valueRef = 0;
    memcpy(&valueRef, &stringObj->data[valueOffset], sizeof(int));

    const PrimitiveArray* arrayHeader = getValue(executor->gc->memoryRegion, valueRef);
    char* path = malloc(arrayHeader->length+1); // remember null terminator
    memcpy(path, arrayHeader->memory, arrayHeader->length);
    path[arrayHeader->length] = 0;

    int result = 0;
    if (access(path, F_OK) == 0) {
        result = 1;
    } else {
        result = 0;
    }

    free(path);
    return result;
}
