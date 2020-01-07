#pragma once

#include <SFML/Graphics.hpp>
#include "game_data.h"
#include "rectangle.h"

class MainMenuState : public State {
private:
	GameDataRef _data;

	sf::Text _title;
	sf::Text _playButton;
	sf::Text _quitButton;
	
	Rectangle getTitleRect();
	Rectangle getButtonsRect();
public:
	MainMenuState(GameDataRef data);
	~MainMenuState();

	void init() override;

	void handleInput() override;
	void update(float dt) override;
	void draw(float dt) override;
};
