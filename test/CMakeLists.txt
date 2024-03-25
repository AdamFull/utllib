cmake_minimum_required(VERSION 3.18)
project(utility_test)

file(GLOB_RECURSE UTILITY_TEST_SOURCES *.c *.cpp *.cc)
file(GLOB_RECURSE UTILITY_TEST_HEADERS *.h *.hpp *.hh)

add_executable(${PROJECT_NAME} ${UTILITY_TEST_SOURCES} ${UTILITY_TEST_HEADERS})

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${UTILITY_LIB_ROOT}/other"
    "${UTILITY_LIB_ROOT}/source"
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${UTILITY_LIB_ROOT}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE utility_lib
)