#include <cmath>
#include <utility>
#include "game_state.h"

GameState::GameState(GameDataRef data) : 
	_data(data), 
	_map(1, 1, GameMapObjects::WALL) 
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


size_t GameState::_getMainScreenOffset() {
	return _mapBlockSize * _map.getN();
}

std::pair<size_t, size_t> GameState::_getGameScreenSize() {
	auto screenSize { _data->window.getSize() };
	return std::pair(screenSize.x - _getMainScreenOffset(), screenSize.y);
}

std::tuple<float, float, float> GameState::_calcNearestWall(float angle) {
	float t = 0;
	float cx = _player.x;
	float cy = _player.y;
	for (; t < _map.getN(); t += 0.01) {
		cx = _player.x + t * std::cos(angle);
		cy = _player.y + t * std::sin(angle);
		if (_map(int(cx), int(cy)) == GameMapObjects::WALL)
			break;
	}
	return std::tuple(cx, cy, t);
}

void GameState::_draw() {
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(_mapBlockSize, _mapBlockSize));
	rect.setFillColor(sf::Color::White);

	for (size_t i = 0; i < _map.getN(); i++)
		for (size_t j = 0; j < _map.getN(); j++)
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

		float wallHeight = height / (c * std::cos(angle - _player.angle));

		line[0].position = sf::Vector2f(_getMainScreenOffset() + i, height / 2 - wallHeight / 2);
		line[1].position = sf::Vector2f(_getMainScreenOffset() + i, height / 2 + wallHeight / 2);
		line[0].color = line[1].color = sf::Color::Green;
		_data->window.draw(line);
	}
}

void GameState::init() {
	_player.x = 1.5;
	_player.y = 1.5;
	_player.angle = 0;

	_mapManager.loadMap("First", "third.map");

	_map = _mapManager.getMap("First");
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
