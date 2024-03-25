cmake_minimum_required(VERSION 3.18)
project(utility_container)

file(GLOB_RECURSE UTILITY_CONTAINER_SOURCES *.cpp)
file(GLOB_RECURSE UTILITY_CONTAINER_HEADERS *.h *.hpp *.hh)

add_library(${PROJECT_NAME} STATIC ${UTILITY_CONTAINER_SOURCES} ${UTILITY_CONTAINER_HEADERS})

target_include_directories(${PROJECT_NAME}
	PUBLIC "${UTILITY_LIB_ROOT}/source"
)