//
// Created by landry on 8/29/22.
//

#include "utils/classpathhelper.h"

#include <stdlib.h>
#include <string.h>

char* getPath(const char* classPath, const char* className) {
    int dirLen = strlen(classPath);
    int nameLen = strlen(className);
    // dir + name + .class (6 chars) + null terminator
    char* result = malloc(dirLen + nameLen + 6 + 1);

    strcpy(result, classPath);
    strcat(result, className);
    strcat(result, ".class");

    return result;
}
