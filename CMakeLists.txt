cmake_minimum_required(VERSION 3.10)
project(lab03)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(main
    main.cpp
    src/figure.cpp
    src/hexagon.cpp
    src/octagon.cpp
    src/triangle.cpp
    src/array.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(main PRIVATE DEBUG)
endif()

find_package(GTest REQUIRED)

add_executable(tests
    src/test/test.cpp
    src/figure.cpp
    src/hexagon.cpp
    src/octagon.cpp
    src/triangle.cpp
    src/array.cpp
)

target_include_directories(tests PRIVATE include)
target_link_libraries(tests GTest::gtest GTest::gtest_main)

enable_testing()
add_test(NAME lab03_tests COMMAND tests)