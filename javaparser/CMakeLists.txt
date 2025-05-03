add_library(parser ${CMAKE_CURRENT_LIST_DIR}/src/javaparser.c)

target_link_libraries(parser common)
target_include_directories(parser PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(parser_main ${CMAKE_CURRENT_LIST_DIR}/src/main.c)

target_link_libraries(parser_main parser)
