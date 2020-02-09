cmake_minimum_required(VERSION 3.0)

project(Shooter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Shooter 
	asset_manager.cpp
	game.cpp
	input_manager.cpp
	main.cpp
	main_menu_state.cpp
	state_machine.cpp
)

find_package(SFML 2.5 REQUIRED graphics)
if (SFML_FOUND)
	target_link_libraries(Shooter sfml-graphics)
endif()
