cmake_minimum_required(VERSION 3.10)
project(DungeonEditor)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# Основная программа
add_executable(DungeonEditor
    main.cpp
    src/npc_types.cpp
    src/factory.cpp
    src/visitor.cpp
    src/observer.cpp
)
