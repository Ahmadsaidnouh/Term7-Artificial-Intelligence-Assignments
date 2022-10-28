cmake_minimum_required(VERSION 3.5)

project(Astar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Astar main.cpp utilities.cpp A_star.cpp)

install(TARGETS Astar
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})