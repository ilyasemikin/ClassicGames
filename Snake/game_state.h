#pragma once

#include <SFML/Graphics.hpp>
#include "game_data.h"
#include "rectangle.h"
#include "state.h"

class GameState : public State {
private:
	GameDataRef _data;

	sf::Text _score;
	sf::RectangleShape _fieldBoundary;
	sf::RectangleShape _oddEmptyCell;
	sf::RectangleShape _evenEmptyCell;

	struct {
		size_t width;
		size_t height;
	} _field;

	Rectangle getScoreRect();
	Rectangle getMainRect();

	float getBlockSize(const Rectangle &rect);
public:
	GameState(GameDataRef data, size_t width, size_t height);
	~GameState();

	void init() override;
	
	void handleInput() override;
	void update(float dt) override;
	void draw(float dt) override;
};
