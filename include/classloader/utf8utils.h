#ifndef JVM_PROJECT_UTF8UTILS_H
#define JVM_PROJECT_UTF8UTILS_H

#include "constantparser.h"

char* utf82cstring(UTF8* utf8);

int isEqual(UTF8*, const char*);

int isEqualUtf8(UTF8*, UTF8*);

#endif //JVM_PROJECT_UTF8UTILS_H
