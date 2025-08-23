#ifndef LOG_H
#define LOG_H

typedef enum {
    NONE = 0,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    TRACE,
    VERBOSE
} LogLevel;

void errorLog(const char* text, ...);
void warningLog(const char* text, ...);
void infoLog(const char* text, ...);
void debugLog(const char* text, ...);
void traceLog(const char* text, ...);
void verboseLog(const char* text, ...);

void setLogLevel(LogLevel logLevel);
LogLevel getLogLevel();

#endif //LOG_H
