cmake_minimum_required(VERSION 3.0)

project(TetrisGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TetrisGame main.cpp game.cpp game_scene.cpp png_image.cpp config_parser.cpp string_operations.cpp)

find_package(SFML 2.5 REQUIRED graphics)
if (SFML_FOUND)
	target_link_libraries(TetrisGame sfml-graphics)
endif()
