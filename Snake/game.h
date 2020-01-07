#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>
#include "asset_manager.h"
#include "game_data.h"
#include "input_manager.h"
#include "state_machine.h"

class Game {
private:
	GameDataRef _data;

	float deltaTime;

	sf::Clock _clock;
public:
	Game(size_t width, size_t height, const std::string &title);

	void run();
};
