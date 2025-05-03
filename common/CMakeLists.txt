
file(GLOB COMMON_SRCS
        "${CMAKE_CURRENT_LIST_DIR}/src/classloader/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/src/classpool/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/src/heap/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/src/interpreter/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/src/memory/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/src/stack/*.c"
        "${CMAKE_CURRENT_LIST_DIR}/src/utils/*.c"
)

add_library(common ${COMMON_SRCS})

target_link_libraries(common m)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
