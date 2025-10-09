cmake_minimum_required(VERSION 3.10)

project(MoneyClass LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(money_app
    main.cpp
    src/money.cpp
)

find_package(GTest REQUIRED)

add_executable(money_test
    test/test.cpp
    src/money.cpp
)

target_link_libraries(money_test GTest::gtest GTest::gtest_main)

enable_testing()
add_test(NAME money_test COMMAND money_test)