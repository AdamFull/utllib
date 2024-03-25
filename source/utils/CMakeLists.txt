cmake_minimum_required(VERSION 3.18)
project(utility_utils)

file(GLOB_RECURSE UTILITY_UTILS_SOURCES *.cpp)
file(GLOB_RECURSE UTILITY_UTILS_HEADERS *.h *.hpp *.hh)

add_library(${PROJECT_NAME} STATIC ${UTILITY_UTILS_SOURCES} ${UTILITY_UTILS_HEADERS})

target_include_directories(${PROJECT_NAME}
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
	PUBLIC "${UTILITY_LIB_ROOT}/source"
)