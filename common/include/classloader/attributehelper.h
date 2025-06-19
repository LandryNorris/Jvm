#ifndef JVM_PROJECT_ATTRIBUTEHELPER_H
#define JVM_PROJECT_ATTRIBUTEHELPER_H

#include "attributeloader.h"

void printCodeAttributes(Code* code);
void printFileAttributes(ConstantPool* constantPool, AttributePool* attributePool);

#endif // JVM_PROJECT_ATTRIBUTEHELPER_H
