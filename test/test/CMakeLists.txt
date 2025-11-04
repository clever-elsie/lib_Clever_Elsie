cmake_minimum_required(VERSION 3.20)
project(test_lib_Clever_Elsie VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -flto -march=native -DNDEBUG")

add_executable(test src/main.cpp)
include_directories(../../)
include_directories(include)
file(GLOB_RECURSE SOURCES "*.cpp")
