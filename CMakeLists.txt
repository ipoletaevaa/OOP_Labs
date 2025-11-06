cmake_minimum_required(VERSION 3.10)
project(FiguresLab4)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(main main.cpp)
add_executable(tests tests.cpp)