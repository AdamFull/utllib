cmake_minimum_required(VERSION 3.18)
project(utility_lib)

add_subdirectory(container)
add_subdirectory(debugging)
add_subdirectory(design_pattern)
add_subdirectory(filesystem)
add_subdirectory(logger)
add_subdirectory(parsing)
add_subdirectory(plugin)
add_subdirectory(reactive)
add_subdirectory(threading)
add_subdirectory(utils)

file(GLOB UTILITY_LIB_SOURCES *.cpp)

add_library(${PROJECT_NAME} STATIC ${UTILITY_LIB_SOURCES})

target_link_libraries(${PROJECT_NAME}
    PUBLIC mimalloc
    PRIVATE utility_container
    PRIVATE utility_debugging
    PRIVATE utility_design_pattern
    PRIVATE utility_filesystem
    PRIVATE utility_logger
    PRIVATE utility_parsing
    PRIVATE utility_plugin
    PRIVATE utility_reactive
    PRIVATE utility_threading
    PRIVATE utility_utils
)