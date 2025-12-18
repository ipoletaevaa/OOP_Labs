cmake_minimum_required(VERSION 3.15)
project(DungeonGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

include_directories(include)

set(SRC_FILES
    src/factory.cpp
    src/game.cpp
    src/npc_types.cpp
    src/observer.cpp
)

add_executable(DungeonGame main.cpp ${SRC_FILES})
target_link_libraries(DungeonGame Threads::Threads)

target_compile_features(DungeonGame PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "9")
    target_link_libraries(DungeonGame stdc++fs)
endif()

find_package(GTest QUIET)
if(GTest_FOUND)
    add_executable(UnitTests tests.cpp ${SRC_FILES})
    target_link_libraries(UnitTests GTest::gtest GTest::gtest_main Threads::Threads)
    
    add_executable(IntegrationTests integration_tests.cpp ${SRC_FILES})
    target_link_libraries(IntegrationTests GTest::gtest GTest::gtest_main Threads::Threads)
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "9")
        target_link_libraries(UnitTests stdc++fs)
        target_link_libraries(IntegrationTests stdc++fs)
    endif()
    
    enable_testing()
    add_test(NAME UnitTests COMMAND UnitTests)
    add_test(NAME IntegrationTests COMMAND IntegrationTests)
endif()