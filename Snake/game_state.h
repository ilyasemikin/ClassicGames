#pragma once

#include <list>
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
		Rectangle topRect;
		Rectangle mainRect;
		float blockSize;
	} _output;

	struct {
		size_t width;
		size_t height;
	} _field;

	struct {
		sf::Clock clock;
		float time;
	} _deltaTime;

	enum Direction { 
		UP, DOWN, LEFT, RIGHT
	};

	struct {
		sf::Vector2u head;
		std::list<sf::Vector2u> points;
		Direction direction;
	} _snake;

	size_t _scoreValue;

	Rectangle getTopRect();
	Rectangle getMainRect();

	float getBlockSize();

	bool isGameOver();
	sf::Vector2u getNewHead();
	void changeDirection(Direction newDirection);
	void makeStep();

	void updateDeltaTime();

	void drawScore();
	void drawField();
	void drawSnake();
public:
	GameState(GameDataRef data, size_t width, size_t height);
	~GameState();

	void init() override;

	void handleInput() override;
	void update(float dt) override;
	void draw(float dt) override;
};
