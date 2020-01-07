#include "game_state.h"

GameState::GameState(GameDataRef data, size_t width, size_t height) : _data(data) {
	_field.width = width;
	_field.height = height;
}

GameState::~GameState() {

}

Rectangle GameState::getTopRect() {
	Rectangle ret;
	auto wndSize { _data->window.getSize() };
	ret.x = 0;
	ret.y = 0;
	ret.width = wndSize.x;
	ret.height = wndSize.y * 0.05;
	return ret;
}

Rectangle GameState::getMainRect() {
	Rectangle ret;
	auto wndSize { _data->window.getSize() };
	ret.x = 0;
	ret.y = wndSize.y * 0.05 + 1;
	ret.width = wndSize.x;
	ret.height = wndSize.y - ret.y;
	return ret;
}

float GameState::getBlockSize() {
	auto widthRatio { 
		static_cast<float>(_field.width) / _output.mainRect.width 
	};
	auto heightRation {
		static_cast<float>(_field.height) / _output.mainRect.height
	};
	if (widthRatio < heightRation)
		return static_cast<float>(_output.mainRect.height) / _field.height;
	else
		return static_cast<float>(_output.mainRect.width) / _field.width;
}

bool GameState::isGameOver() {
	auto newHead { getNewHead() };

	if (newHead.x == 0 || newHead.x == _field.width - 1 ||
	    newHead.y == 0 || newHead.y == _field.height - 1)
		return true;

	for (auto &point : _snake.points)
		if (point.x == newHead.x && point.y == newHead.y)
			return true;

	return false;
}

sf::Vector2u GameState::getNewHead() {
	const std::map<Direction, sf::Vector2i> offsets {
		{ Direction::DOWN, { 0, 1 } },
		{ Direction::LEFT, { -1, 0 } },
		{ Direction::RIGHT, { 1, 0 } },
		{ Direction::UP, { 0, -1 } }
	};

	auto offset { offsets.at(_snake.direction) };
	return sf::Vector2u(_snake.head.x + offset.x, _snake.head.y + offset.y); 
}

void GameState::changeDirection(Direction newDirection) {
	switch (newDirection) {
		case Direction::DOWN:
		case Direction::UP:
			if (_snake.direction == Direction::LEFT || 
			    _snake.direction == Direction::RIGHT)
				_snake.direction = newDirection;
			break;
		case Direction::LEFT:
		case Direction::RIGHT:
			if (_snake.direction == Direction::UP ||
			    _snake.direction == Direction::DOWN)
				_snake.direction = newDirection;
			break;
	}
}

void GameState::makeStep() {
	if (isGameOver())
		return;

	auto newHead { getNewHead() };
	_snake.points.push_front(_snake.head);
	_snake.points.pop_back();
	_snake.head = newHead;
}

void GameState::updateDeltaTime() {
	_deltaTime.time = _deltaTime.clock.getElapsedTime().asSeconds();
}

void GameState::drawScore() {
	auto str { "Score: " + std::to_string(_scoreValue) };
	_score.setString(str);
	_data->window.draw(_score);
}

void GameState::drawField() {
	auto isBoundaryCell {
		[&](size_t x, size_t y) {
			return x == 0 || x == _field.width - 1 || y == 0 || y == _field.height - 1;
		}
	};
	sf::RectangleShape rect;
	for (size_t i = 0; i < _field.width; i++)
		for (size_t j = 0; j < _field.height; j++) {
			if (isBoundaryCell(i, j))
				rect = _fieldBoundary;
			else if ((i + j) % 2 != 0)
				rect = _oddEmptyCell;
			else
				rect = _evenEmptyCell;
			
			rect.setPosition(
				_output.mainRect.x + i * _output.blockSize,
				_output.mainRect.y + j * _output.blockSize
			);
			_data->window.draw(rect);
		}
}

void GameState::drawSnake() {
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Blue);
	rect.setSize(sf::Vector2f(_output.blockSize, _output.blockSize));
	
	rect.setPosition(
		sf::Vector2f(
			_output.mainRect.x + _snake.head.x * _output.blockSize,
			_output.mainRect.y + _snake.head.y * _output.blockSize
		)
	);
	_data->window.draw(rect);
	
	for (auto &point : _snake.points) {
		auto x { _output.mainRect.x + point.x * _output.blockSize };
		auto y { _output.mainRect.y + point.y * _output.blockSize };
		rect.setPosition(sf::Vector2f(x, y));
		_data->window.draw(rect);
	}
}

void GameState::init() {
	_scoreValue = 0;
	_output.topRect = getTopRect();
	_output.mainRect = getMainRect();
	_output.blockSize = getBlockSize();

	for (size_t i = 1; i < 8; i++)
		_snake.points.push_front(sf::Vector2u(i, _field.height / 2));
	_snake.head = sf::Vector2u(8, _field.height / 2);
	_snake.direction = Direction::RIGHT;

	_data->assets.loadFont("RetroVilleNC", "Fonts/RetroVilleNC-Regular.ttf");

	_score.setFont(_data->assets.getFont("RetroVilleNC"));
	_score.setFillColor(sf::Color::White);
	_score.setString("Score:");
	_score.setCharacterSize(_output.topRect.height * 0.95);

	_fieldBoundary.setFillColor(sf::Color(0, 64, 0));
	_fieldBoundary.setSize(sf::Vector2f(_output.blockSize, _output.blockSize));

	_oddEmptyCell.setFillColor(sf::Color(0, 128, 0));
	_oddEmptyCell.setSize(sf::Vector2f(_output.blockSize, _output.blockSize));

	_evenEmptyCell.setFillColor(sf::Color(0, 192, 0));
	_evenEmptyCell.setSize(sf::Vector2f(_output.blockSize, _output.blockSize));
}

void GameState::handleInput() {
	sf::Event event;

	while (_data->window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				_data->window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Up)
					changeDirection(Direction::UP);
				else if (event.key.code == sf::Keyboard::Down)
					changeDirection(Direction::DOWN);
				else if (event.key.code == sf::Keyboard::Left)
					changeDirection(Direction::LEFT);
				else if (event.key.code == sf::Keyboard::Right)
					changeDirection(Direction::RIGHT);
				break;
			default:
				break;
		}
	}
}

void GameState::update(float dt) {
	updateDeltaTime();

	if (_deltaTime.time > 0.1) {
		makeStep();
		_deltaTime.clock.restart();
	}
}

void GameState::draw(float dt) {
	_data->window.clear();

	drawScore();
	drawField();
	drawSnake();

	_data->window.display();
}
