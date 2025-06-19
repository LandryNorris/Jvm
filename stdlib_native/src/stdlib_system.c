
#include <stdio.h>
#include <string.h>

#include "../../javarunner/include/execution_engine/executor.h"
#include "classloader/classloader.h"
#include "memory/objheader.h"
#include "memory/primitive_array.h"
#include "synthetic/class_creator.h"

void Java_java_lang_Printer_print() {
    printf("System is printing!\n");
}

void Java_java_lang_SyntheticPrinter_print(uint32_t obj, uint32_t text) {
    Executor* executor = getMainExecutor();
    ObjHeader* stringObj = getValue(executor->gc->memoryRegion, (int)text);

    // For Strings, the value is fields[0]
    int valueOffset = stringObj->fields[0]->offset;
    int valueRef = 0;
    memcpy(&valueRef, &stringObj->data[valueOffset], sizeof(int));

    const PrimitiveArray* arrayHeader = getValue(executor->gc->memoryRegion, valueRef);

    fwrite(arrayHeader->memory, sizeof(char), arrayHeader->length, stdout);
}

void Java_java_lang_System_setupPrinter() {
    printf("Initializing printer!\n");

    const Executor* executor = getMainExecutor();

    uint8_t loadedFresh;
    ClassFile* printStreamClass = getClassFile(executor->loader, "java/io/PrintStream", &loadedFresh);

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
    printf("Found system\n");
}
