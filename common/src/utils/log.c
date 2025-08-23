#include "utils/log.h"

#include <stdio.h>
#include <stdarg.h>

static LogLevel currentLogLevel = NONE;

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
    printf("[ERROR]: ");
    vprintf(text, args);
    va_end(args);
}

void traceLog(const char* text, ...) {
    if (currentLogLevel < TRACE) {
        return;
    }
    va_list args;
    va_start(args, text);
    printf("[ERROR]: ");
    vprintf(text, args);
    va_end(args);
}

void verboseLog(const char* text, ...) {
    if (currentLogLevel < VERBOSE) {
        return;
    }
    va_list args;
    va_start(args, text);
    printf("[ERROR]: ");
    vprintf(text, args);
    va_end(args);
}
