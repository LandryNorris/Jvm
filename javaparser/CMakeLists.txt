file(GLOB PARSER_SRCS "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

add_executable(parser ${PARSER_SRCS})

target_link_libraries(parser common)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
