cmake_minimum_required(VERSION 3.28)

project(JVM C CXX ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBFFI REQUIRED IMPORTED_TARGET libffi)

# Or with a friendly error message
if (NOT LIBFFI_FOUND)
    message(FATAL_ERROR "libffi is required. Install it with your package manager (e.g., `apt install libffi-dev`)")
endif()

add_subdirectory(stdlib)
add_subdirectory(common)
# Native portion of the stdlib. It may depend on common in the future
add_subdirectory(stdlib_native)

add_subdirectory(javarunner)
add_subdirectory(javaparser)

add_subdirectory(integration_tests/parser_tests)
