cmake_minimum_required(VERSION 3.18)
project(utl_library_project)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_STANDARD_REQUIRED ON)

set(UTILITY_LIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Backward CONFIG REQUIRED)
find_package(mimalloc REQUIRED NO_SYSTEM_ENVIRONMENT_PATH)

add_subdirectory(source)
#add_subdirectory(test)