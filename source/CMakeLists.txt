cmake_minimum_required(VERSION 3.18)
project(utility_lib)

add_subdirectory(design_pattern)
add_subdirectory(reactive)

file(GLOB UTILITY_LIB_SOURCES *.cpp)

add_library(${PROJECT_NAME} STATIC ${UTILITY_LIB_SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE utility_design_pattern
    PRIVATE utility_reactive
)