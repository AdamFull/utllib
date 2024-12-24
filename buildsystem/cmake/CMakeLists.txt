cmake_minimum_required(VERSION 3.18)
project(utility_lib)

set(UTILITY_SOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")

file(GLOB_RECURSE UTILITY_MEMORY_SOURCES "${UTILITY_SOURCES_PATH}/source/*.cpp" "${UTILITY_SOURCES_PATH}/source/*.hpp" "${UTILITY_SOURCES_PATH}/source/*.h")

add_library(${PROJECT_NAME} ${UTILITY_MEMORY_SOURCES})

target_include_directories(${PROJECT_NAME} 
	PUBLIC "${UTILITY_SOURCES_PATH}/source/"
	PUBLIC "${UTILITY_SOURCES_PATH}/other/"
)