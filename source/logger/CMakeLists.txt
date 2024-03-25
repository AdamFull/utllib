cmake_minimum_required(VERSION 3.18)
project(utility_logger)

file(GLOB_RECURSE UTILITY_LOGGER_SOURCES *.cpp)
file(GLOB_RECURSE UTILITY_LOGGER_HEADERS *.h *.hpp *.hh)

add_library(${PROJECT_NAME} STATIC ${UTILITY_LOGGER_SOURCES} ${UTILITY_LOGGER_HEADERS})

target_link_libraries(${PROJECT_NAME}
    PUBLIC Backward::Backward
)

target_include_directories(${PROJECT_NAME}
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
	PUBLIC "${UTILITY_LIB_ROOT}/source"
)