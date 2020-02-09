#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "asset_manager.h"
#include "input_manager.h"
#include "state_machine.h"

struct GameData {
	sf::RenderWindow window;
	AssetManager assets;
	InputManager input;
	StateMachine machine;
};

using GameDataRef = std::shared_ptr<GameData>;
