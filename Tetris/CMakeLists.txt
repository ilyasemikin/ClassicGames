cmake_minimum_required(VERSION 3.0)

project(TetrisGame LANGUAGES C CXX)

add_executable(TetrisGame main.cpp game.cpp)

find_package(SFML 2.5 REQUIRED graphics)
if (SFML_FOUND)
	target_link_libraries(TetrisGame sfml-graphics)
endif()
