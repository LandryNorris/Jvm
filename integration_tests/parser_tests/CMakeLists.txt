
file(GLOB PARSER_TEST_SRCS ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

add_executable(parser_tests ${PARSER_TEST_SRCS})

target_link_libraries(parser_tests GTest::gtest)
target_link_libraries(parser_tests parser)
