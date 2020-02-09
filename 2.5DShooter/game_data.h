#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "asset_manager.h"
#include "state_machine.h"

struct GameData {
	sf::RenderWindow window;
	AssetManager assets;
	StateMachine machine;
};

using GameDataRef = std::shared_ptr<GameData>;
