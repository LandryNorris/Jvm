#ifndef JVM_PROJECT_UTF8UTILS_H
#define JVM_PROJECT_UTF8UTILS_H

#include "classloader.h"
#include "constantparser.h"
#include "memory/garbagecollector.h"

char* utf82cstring(UTF8* utf8);
int utf82string(GarbageCollector* gc, const ClassLoader* loader, const UTF8* utf8);

int isEqual(UTF8*, const char*);

int isEqualUtf8(UTF8*, UTF8*);

#endif //JVM_PROJECT_UTF8UTILS_H
