#pragma once

#include <SFML/Graphics.hpp>
#include "game_data.h"
#include "rectangle.h"

class GameOverState : public State {
private:
	GameDataRef _data;

    size_t _score;

	sf::Text _gameOverText;
    sf::Text _scoreText;
    sf::Text _retryButton;
    sf::Text _quitButton;
public:
	GameOverState(GameDataRef data, size_t score);
	~GameOverState();

	void init() override;

	void handleInput() override;
	void update(float dt) override;
	void draw(float dt) override;
};
