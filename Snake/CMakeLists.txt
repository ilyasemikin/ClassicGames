cmake_minimum_required(VERSION 3.0)

project(SnakeGame LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SnakeGame main.cpp asset_manager.cpp game.cpp input_manager.cpp state_machine.cpp main_menu_state.cpp game_state.cpp pause_state.cpp game_over_state.cpp)

find_package(SFML 2.5 REQUIRED graphics)
if (SFML_FOUND)
	target_link_libraries(SnakeGame sfml-graphics)
endif()
