#include <cmath>
#include <utility>
#include "game_state.h"

GameState::GameState(GameDataRef data) : 
	_data(data), 
	_map(_mapProperties.size, _mapProperties.size, GameMapObjects::WALL) 
{

}

// TODO: Добавить нормализацию
void GameState::_rotatePlayer(float angle) {
	_player.angle += angle;
}

void GameState::_movePlayer(float c) {
	auto x { _player.x + c * std::cos(_player.angle) };
	auto y { _player.y + c * std::sin(_player.angle) };
	if (_map(int(x), int(y)) == GameMapObjects::EMPTY) {
		_player.x = x;
		_player.y = y;
	}

}

std::pair<size_t, size_t> GameState::_getGameScreenSize() {
	auto screenSize { _data->window.getSize() };
	return std::pair(screenSize.x - _mainScreenOffset, screenSize.y);
}

std::tuple<float, float, float> GameState::_calcNearestWall(float angle) {
	for (float t = 0; t < _mapProperties.size; t += 0.01) {
		float cx = _player.x + t * std::cos(angle);
		float cy = _player.y + t * std::sin(angle);
		if (_map(int(cx), int(cy)) == GameMapObjects::WALL)
			return std::tuple(cx, cy, t);
	}
	return std::tuple(0, 0, 0);
}

void GameState::_draw() {
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(_mapProperties.blockSize, _mapProperties.blockSize));
	rect.setFillColor(sf::Color::White);

	for (size_t i = 0; i < _mapProperties.size; i++)
		for (size_t j = 0; j < _mapProperties.size; j++)
			if (_map(i, j) == GameMapObjects::WALL) {
				rect.setPosition(i * _mapProperties.blockSize, j * _mapProperties.blockSize);
				_data->window.draw(rect);
			}

	auto [width, height] { _getGameScreenSize() };
	for (size_t i = 0; i < width; i++) {
		float angle = _player.angle - _player.fov / 2 + _player.fov * i / float(width);
		auto [cx, cy, c] { _calcNearestWall(angle) };

		sf::VertexArray line(sf::Lines, 2);
		line[0].position = sf::Vector2f(_player.x * _mapProperties.blockSize, _player.y * _mapProperties.blockSize);
		line[1].position = sf::Vector2f(cx * _mapProperties.blockSize, cy * _mapProperties.blockSize);
		line[0].color = line[1].color = sf::Color::Yellow;
		_data->window.draw(line);

		float wallHeight = height / (c * std::cos(angle - _player.angle));

		line[0].position = sf::Vector2f(_mainScreenOffset + i, height / 2 - wallHeight / 2);
		line[1].position = sf::Vector2f(_mainScreenOffset + i, height / 2 + wallHeight / 2);
		line[0].color = line[1].color = sf::Color::Green;
		_data->window.draw(line);
	}
}

void GameState::init() {
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

void GameState::handleInput() {
	sf::Event event;
	while (_data->window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::EventType::Closed:
				_data->window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Right)
					_rotatePlayer(M_PI / 50);
				else if (event.key.code == sf::Keyboard::Left)
					_rotatePlayer(-M_PI / 50);
				else if (event.key.code == sf::Keyboard::Up)
					_movePlayer(0.1);
				else if (event.key.code == sf::Keyboard::Down) 
					_movePlayer(-0.1);
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
