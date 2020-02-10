#include <cmath>
#include <utility>
#include "game_state.h"

#include <iostream>

GameState::GameState(GameDataRef data) : _data(data), _map(_mapSize, _mapSize, GameMapObjects::WALL) {
	const std::vector<std::pair<size_t, size_t>> wallDots {
		{ 1, 1 }, { 1, 2 }, { 1, 3 },
	};

	_player.x = 1.5;
	_player.y = 1.5;
	_player.angle = 0;

	for (size_t i = 0; i < wallDots.size(); i++) {
		auto [x, y] { wallDots[i] };
		_map(x, y) = GameMapObjects::EMPTY;
	}
}

std::pair<float, float> GameState::_calcNearestWall() {
	for (float t = 0; t < _mapSize; t += 0.05) {
		float cx = _player.x + t * std::cos(_player.angle);
		float cy = _player.y + t * std::sin(_player.angle);
		if (_map(int(cx), int(cy)) == GameMapObjects::WALL)
			return std::pair(cx, cy);
	}
	return std::pair(0, 0);
}

void GameState::_drawMap() {
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(_mapBlockSize, _mapBlockSize));
	rect.setFillColor(sf::Color::White);

	for (size_t i = 0; i < _mapSize; i++)
		for (size_t j = 0; j < _mapSize; j++)
			if (_map(i, j) == GameMapObjects::WALL) {
				rect.setPosition(i * _mapBlockSize, j * _mapBlockSize);
				_data->window.draw(rect);
			}
	auto [cx, cy] { _calcNearestWall() };

	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(_player.x * _mapBlockSize, _player.y * _mapBlockSize);
	line[1].position = sf::Vector2f(cx * _mapBlockSize, cy * _mapBlockSize);
	line[0].color = line[1].color = sf::Color::Yellow;
	_data->window.draw(line);
}

void GameState::init() {

}

void GameState::handleInput() {
	sf::Event event;
	while (_data->window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::EventType::Closed:
				_data->window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Down)
					_player.angle = (_player.angle > 360) ? 0 : _player.angle + 0.05; 
				break;
			default:
				break;
		}
	}
}

void GameState::update(float dt) {

}

void GameState::draw(float dt) {
	_data->window.clear();

	_drawMap();

	_data->window.display();
}
