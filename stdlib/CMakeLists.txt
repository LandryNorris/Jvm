
include(FindJava)
include(UseJava)

find_package(Java 1.8 REQUIRED)
find_package(Java COMPONENTS Runtime)
find_package(Java COMPONENTS Development)

set(CMAKE_JAVA_COMPILE_FLAGS "--target"  "8" "--source" "8")

file(GLOB_RECURSE JAVA_SRCS "${CMAKE_CURRENT_LIST_DIR}/src/main/java/**/*.java")

add_jar(java_stdlib SOURCES
        "${CMAKE_CURRENT_LIST_DIR}/src/main/java/java/lang/String.java"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/java/java/lang/Comparable.java"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/java/java/lang/CharSequence.java"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/java/java/lang/System.java"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/java/java/lang/StringBuilder.java"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/java/java/lang/Object.java"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/java/java/io/PrintStream.java"
        "${CMAKE_CURRENT_LIST_DIR}/src/main/java/java/io/Serializable.java"
)

get_target_property(java_stdlib_CLASSDIR java_stdlib CLASSDIR)

add_custom_command(TARGET java_stdlib POST_BUILD
        # The /java suffix is copied on both to avoid copying other files.
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${java_stdlib_CLASSDIR}/java ${CMAKE_SOURCE_DIR}/java-prebuilt/java
)
