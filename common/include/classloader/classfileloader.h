#ifndef JVM_PROJECT_CLASSFILELOADER_H
#define JVM_PROJECT_CLASSFILELOADER_H

#include <utils/arrayutils.h>
#include <classloader/classfile.h>

ClassFile* loadClassFile(const char* classFilePath);

int loadClassFilesRecursive(const char* classFileDirectory);

CStringArray* getDependencyClasses(ClassFile*);

int findClassFile(const char* classFileDirectory, const char* name);

void freeConstantPool(ConstantPool*);

void freeMethodPool(MethodPool*);

void freeAttributePool(AttributePool *attributePool);

void freeFieldPool(FieldPool *fieldPool);

void freeInterfacePool(InterfacePool *interfacePool);

void freeClassFile(ClassFile* classFile);

void freeStackMapTable(StackMapTable* stackMapTable);

#endif //JVM_PROJECT_CLASSFILELOADER_H
