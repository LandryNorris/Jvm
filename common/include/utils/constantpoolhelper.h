#ifndef JVM_PROJECT_CONSTANTPOOLHELPER_H
#define JVM_PROJECT_CONSTANTPOOLHELPER_H

#include "classloader/constantparser.h"

char* parseUTF8(UTF8* utf8);

char* parseUTF8ByIndex(int index, const ConstantPool* constantPool);

char* parseString(String* string, const ConstantPool* constantPool);

char* parseClass(Class* class, const ConstantPool* constantPool);

UTF8* parseClassToUTF8(Class* class, const ConstantPool* constantPool);

UTF8* parseClassToUTF8ByIndex(int index, const ConstantPool* constantPool);

char* parseClassByIndex(int index, const ConstantPool* constantPool);

char* parseNameAndType(NameAndTypeIndex* nameAndTypeIndex, const ConstantPool* constantPool);

char* parseNameAndTypeByIndex(int index, const ConstantPool* constantPool);

char* parseMethodRef(MethodRef* methodRef, const ConstantPool* constantPool);

char* parseMethodRefByIndex(int index, const ConstantPool* constantPool);

char* parseMethodHandle(MethodHandle* methodHandle, const ConstantPool* constantPool);

char* parseInteger(Integer* integer);

char* parseFloat(Float* f);

#endif //JVM_PROJECT_CONSTANTPOOLHELPER_H
