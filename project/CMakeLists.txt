cmake_minimum_required(VERSION 3.21)
project(project)

set(CMAKE_CXX_FLAGS "-pthread")

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-pthread)
add_compile_options(-Wall)
add_executable(project main.cpp Match.cpp Position.cpp Movement.cpp Board.cpp Player.cpp)
