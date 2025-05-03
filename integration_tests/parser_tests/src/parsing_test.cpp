
#include "javaparser.h"

void testParseJavaClass(const char* classFilePath, const char* goldenFilePath) {
    const char* args[] = { "TODO(Landry): Emulate command line arg", classFilePath };
    javaparser_main(2, const_cast<char**>(args));
}
