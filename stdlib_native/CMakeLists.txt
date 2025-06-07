
file(GLOB STDLIB_NATIVE_SRCS ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

add_library(stdlib_native SHARED ${STDLIB_NATIVE_SRCS})
target_include_directories(stdlib_native PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
