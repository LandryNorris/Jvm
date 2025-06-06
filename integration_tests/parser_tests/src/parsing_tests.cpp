#include <gtest/gtest.h>
#include <filesystem>

namespace fs = std::filesystem;

void testParseJavaClass(const char* classFilePath, const char* goldenFilePath);

std::string getTestProjectsRootPath() {
    return "../../../test_projects";
}

std::string getGoldenFilePath() {
    return "../../../integration_tests/parser_tests/golden";
}

TEST(ParserTests, FibonacciParserTest) {
    const fs::path classFilePath = fs::path(getTestProjectsRootPath()) / "fibonacci/build/classes/java/main/Fibonacci.class";
    const fs::path goldenFilePath = fs::path(getGoldenFilePath()) / "Fibonacci.golden";
    testParseJavaClass(classFilePath.c_str(), goldenFilePath.c_str());
}

TEST(ParserTests, AverageParserTest) {
    const fs::path classFilePath = fs::path(getTestProjectsRootPath()) / "average/build/classes/java/main/Averager.class";
    const fs::path goldenFilePath = fs::path(getGoldenFilePath()) / "Average.golden";
    testParseJavaClass(classFilePath.c_str(), goldenFilePath.c_str());
}

TEST(ParserTests, StringsParserTest) {
    const fs::path classFilePath = fs::path(getTestProjectsRootPath()) / "strings/build/classes/java/main/App.class";
    const fs::path goldenFilePath = fs::path(getGoldenFilePath()) / "Strings.golden";
    testParseJavaClass(classFilePath.c_str(), goldenFilePath.c_str());
}
