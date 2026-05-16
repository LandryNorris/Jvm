
file(GLOB RUNNER_SRCS
        "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
)

add_executable(runner ${RUNNER_SRCS})

target_link_libraries(runner PUBLIC common stdlib_native)
target_include_directories(runner PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
