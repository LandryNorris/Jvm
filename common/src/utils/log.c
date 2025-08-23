#include "utils/log.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static LogLevel currentLogLevel = NONE;

LogLevel getLogLevelByName(const char* name) {
    if (strcmp("none", name) == 0) {
        return NONE;
    }
    if (strcmp("error", name) == 0) {
        return ERROR;
    }
    if (strcmp("warning", name) == 0) {
        return WARNING;
    }
    if (strcmp("info", name) == 0) {
        return INFO;
    }
    if (strcmp("debug", name) == 0) {
        return DEBUG;
    }
    if (strcmp("trace", name) == 0) {
        return TRACE;
    }
    if (strcmp("verbose", name) == 0) {
        return VERBOSE;
    }
    return -1;
}

void setLogLevel(LogLevel logLevel) {
    currentLogLevel = logLevel;
}

LogLevel getLogLevel() {
    return currentLogLevel;
}

// The va_start method initializes args
// ReSharper disable CppLocalVariableMightNotBeInitialized

void errorLog(const char* text, ...) {
    if (currentLogLevel < ERROR) {
        return;
    }
    va_list args;
    va_start(args, text);
    printf("[ERROR]: ");
    vprintf(text, args);
    va_end(args);
}

void warningLog(const char* text, ...) {
    if (currentLogLevel < WARNING) {
        return;
    }
    va_list args;
    va_start(args, text);
    printf("[WARNING]: ");
    vprintf(text, args);
    va_end(args);
}

void infoLog(const char* text, ...) {
    if (currentLogLevel < INFO) {
        return;
    }
    va_list args;
    va_start(args, text);
    printf("[INFO]: ");
    vprintf(text, args);
    va_end(args);
}

void debugLog(const char* text, ...) {
    if (currentLogLevel < DEBUG) {
        return;
    }
    va_list args;
    va_start(args, text);
    printf("[DEBUG]: ");
    vprintf(text, args);
    va_end(args);
}

void traceLog(const char* text, ...) {
    if (currentLogLevel < TRACE) {
        return;
    }
    va_list args;
    va_start(args, text);
    printf("[TRACE]: ");
    vprintf(text, args);
    va_end(args);
}

void verboseLog(const char* text, ...) {
    if (currentLogLevel < VERBOSE) {
        return;
    }
    va_list args;
    va_start(args, text);
    printf("[VERBOSE]: ");
    vprintf(text, args);
    va_end(args);
}
