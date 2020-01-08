#pragma once

#include <SFML/Graphics.hpp>
#include "game_data.h"
#include "state.h"

class PauseState : public State {
private:
	GameDataRef _data;

	sf::Text _pauseText;
	sf::Text _continueButton;
	sf::Text _quitButton;

public:
	PauseState(GameDataRef data);
	~PauseState();

	void init() override;

	void handleInput() override;
	void update(float dt) override;
	void draw(float dt) override;
};
