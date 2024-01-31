cmake_minimum_required(VERSION 3.18)
project(utility_lib)

set(UTILITY_SOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../)

file(GLOB UTILITY_MEMORY_SOURCES ${UTILITY_SOURCES_PATH}*.cpp ${UTILITY_SOURCES_PATH}*.hpp ${UTILITY_SOURCES_PATH}*.h)

add_subdirectory(${UTILITY_SOURCES_PATH}logger/buildsystem/cmake logger)

add_library(${PROJECT_NAME} ${UTILITY_MEMORY_SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC utllogger)