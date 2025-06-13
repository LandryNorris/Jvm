
#include <stdio.h>

#include "../../javarunner/include/execution_engine/executor.h"
#include "classloader/classloader.h"
#include "memory/objheader.h"
#include "synthetic/class_creator.h"

void Java_java_lang_Printer_print() {
    printf("System is printing!\n");
}

void Java_java_lang_System_setupPrinter() {
    printf("Initializing printer!\n");

    const NativeMethodCreationContext nativeMethods[] = {
        {
            .name = "print",
            .descriptor = "(Ljava/lang/String;)V",
        },
    };

    const ClassCreationContext context = {
        .name = "java/lang/SyntheticPrinter",
        .numNativeMethods = 1,
        .nativeMethods = nativeMethods,
    };
    ClassFile* syntheticPrinter = createClassFile(&context);

    const Executor* executor = getMainExecutor();

    addClass(executor->loader->classPool, syntheticPrinter);

    uint8_t loadedFresh;
    ClassFile* system = getClassFile(executor->loader, "java/lang/System", &loadedFresh);

    const int printerObject = createObject(executor->gc, syntheticPrinter);

    setInt32StaticField(system, "out", printerObject);
    printf("Found system\n");
}
