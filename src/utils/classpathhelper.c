//
// Created by landry on 8/29/22.
//

#include "classpathhelper.h"
#include "string.h"

char* getPath(const char* classPath, const char* className) {
    int dirLen = strlen(classPath);
    int nameLen = strlen(className);
    char* result = malloc(dirLen + nameLen + 6);

    strcpy(result, classPath);
    strcat(result, className);
    strcat(result, ".class");

    return result;
}
