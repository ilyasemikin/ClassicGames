#include <cmath>
#include <utility>
#include "game_state.h"

#include <iostream>

GameState::GameState(GameDataRef data) : _data(data), _map(_mapSize, _mapSize, GameMapObjects::WALL) {
	const std::vector<std::pair<size_t, size_t>> wallDots {
		{ 1, 1 }, { 1, 2 }, { 1, 3 },
		{ 2, 1 }, { 2, 2 }, { 2, 3 }
	};

	_player.x = 1.5;
	_player.y = 1.5;
	_player.angle = 0;

	for (size_t i = 0; i < wallDots.size(); i++) {
		auto [x, y] { wallDots[i] };
		_map(x, y) = GameMapObjects::EMPTY;
	}
}

std::pair<size_t, size_t> GameState::_getGameScreenSize() {
	auto screenSize { _data->window.getSize() };
	return std::pair(screenSize.x - _mainScreenOffset, screenSize.y);
}

std::tuple<float, float, float> GameState::_calcNearestWall(float angle) {
	for (float t = 0; t < _mapSize; t += 0.01) {
		float cx = _player.x + t * std::cos(angle);
		float cy = _player.y + t * std::sin(angle);
		if (_map(int(cx), int(cy)) == GameMapObjects::WALL)
			return std::tuple(cx, cy, t);
	}
	return std::tuple(0, 0, 0);
}

void GameState::_draw() {
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(_mapBlockSize, _mapBlockSize));
	rect.setFillColor(sf::Color::White);

	for (size_t i = 0; i < _mapSize; i++)
		for (size_t j = 0; j < _mapSize; j++)
			if (_map(i, j) == GameMapObjects::WALL) {
				rect.setPosition(i * _mapBlockSize, j * _mapBlockSize);
				_data->window.draw(rect);
			}

	auto [width, height] { _getGameScreenSize() };
	for (size_t i = 0; i < width; i++) {
		float angle = _player.angle - _player.fov / 2 + _player.fov * i / float(width);
		auto [cx, cy, c] { _calcNearestWall(angle) };

		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(_player.x * _mapBlockSize, _player.y * _mapBlockSize);
		line[1].position = sf::Vector2f(cx * _mapBlockSize, cy * _mapBlockSize);
		line[0].color = line[1].color = sf::Color::Yellow;
		_data->window.draw(line);

		std::cout << cx  << " " << cy << " " << c << std::endl;

		float wallHeight = height / (c * std::cos(angle - _player.angle));

		line[0].position = sf::Vector2f(_mainScreenOffset + i, height / 2 - wallHeight / 2);
		line[1].position = sf::Vector2f(_mainScreenOffset + i, height / 2 + wallHeight / 2);
		line[0].color = line[1].color = sf::Color::Green;
		_data->window.draw(line);
	}
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
				if (event.key.code == sf::Keyboard::Right)
					_player.angle = (_player.angle > 2 * 3.14) ? 0 : _player.angle + 3.14 / 100;
				else if (event.key.code == sf::Keyboard::Left)
					_player.angle = (_player.angle < 0) ? 2 * 3.14 : _player.angle - 3.14 / 100;
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

	_draw();

	_data->window.display();
}
