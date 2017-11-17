cmake_minimum_required(VERSION 3.8)
project(Yacc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp
        src/LALR.cpp
        )
include_directories(include)
add_executable(Yacc ${SOURCE_FILES})