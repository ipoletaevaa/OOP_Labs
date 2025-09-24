cmake_minimum_required(VERSION 3.10)
project(lab1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Проверка версии GCC
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0)
        message(FATAL_ERROR "Требуется GCC 14 или выше. Текущая версия: ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
endif()

# Включаем папки с исходниками
include_directories(include)

# Основная программа
add_executable(lab1_main 
    src/lab1.cpp
    src/lab1_functions.cpp
)

# Поиск GoogleTest
find_path(GTEST_INCLUDE_DIR gtest/gtest.h
    PATHS /usr/local/include /usr/include
)

find_library(GTEST_LIBRARY
    NAMES gtest
    PATHS /usr/local/lib /usr/lib
)

find_library(GTEST_MAIN_LIBRARY
    NAMES gtest_main
    PATHS /usr/local/lib /usr/lib
)

if (GTEST_INCLUDE_DIR AND GTEST_LIBRARY AND GTEST_MAIN_LIBRARY)
    message(STATUS "Found GTest: ${GTEST_LIBRARY}")
else()
    message(FATAL_ERROR "GTest not found!")
endif()

# Тестовая программа
add_executable(lab1_test 
    tests/test.cpp
    src/lab1_functions.cpp
)

target_include_directories(lab1_test PRIVATE ${GTEST_INCLUDE_DIR})
target_link_libraries(lab1_test ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} pthread)

# Включить тестирование
enable_testing()
add_test(NAME lab1_test COMMAND lab1_test)