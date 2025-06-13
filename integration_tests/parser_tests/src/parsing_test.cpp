
#include <filesystem>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <fstream>
#include <regex>

#include "javaparser.h"

namespace fs = std::filesystem;

std::string tabsToSpaces(std::string input, int numSpaces) {
    const auto spaces = std::string(numSpaces, ' ');

    return std::regex_replace(input, std::regex("\t"), spaces);
}

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void testParseJavaClass(const char* classFilePath, const char* goldenFilePath) {
    testing::internal::CaptureStdout();
    const char* args[] = { "TODO(Landry): Emulate command line arg", classFilePath };
    javaparser_main(2, const_cast<char**>(args));

    const std::string output = testing::internal::GetCapturedStdout();
    const std::string adjustedOutput = tabsToSpaces(output, 4);
    const std::string expected = readFile(goldenFilePath);
    const std::string adjustedExpected = tabsToSpaces(expected, 4);

    ASSERT_EQ(adjustedOutput, adjustedExpected);
}
