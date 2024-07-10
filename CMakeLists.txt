cmake_minimum_required(VERSION 3.28)
project(tinyexpr)

set(CMAKE_CXX_STANDARD 14)

add_executable(tinyexpr main.cpp
        myexpr.h
        myexpr.cpp
        test_myexpr.cpp
        test_te_interp.cpp
        test_te_compile.cpp
        test_te_eval.cpp
        test_next_token.cpp)
