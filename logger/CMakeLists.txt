cmake_minimum_required(VERSION 3.19.1)
project(utllogger)

file(GLOB_RECURSE LOGGER_SOURCES *.cpp *.c *.cc *.h *.hpp)

add_library(${PROJECT_NAME} STATIC ${LOGGER_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC -DMAIN_PROJECT_DIRECTORY_PATH="${MAIN_PROJECT_DIRECTORY_PATH}")