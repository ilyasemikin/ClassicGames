#include "game_over_state.h"
#include "pause_state.h"
#include "settings.h"
#include "game_state.h"

GameState::GameState(GameDataRef data, size_t width, size_t height) : 
	_data(data),
	_generator(_rd())
{	
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

std::pair<float, float> GameState::getBlockPosition(size_t x, size_t y) {
	std::pair<float, float> ret;
	ret.first = _output.mainRect.x + x * _output.blockSize;
	ret.second = _output.mainRect.y + y * _output.blockSize;
	return ret;
}

void GameState::generateFruit() {
	std::uniform_int_distribution<size_t> xDist(1, _field.width - 2);
	std::uniform_int_distribution<size_t> yDist(1, _field.height - 2);
	while  (true) {
		size_t x = xDist(_generator);
		size_t y = yDist(_generator);
		bool isEmpty = true;
		for (auto &point : _snake.points)
			if (point.x == x && point.y == y) {
				isEmpty = false;
				break;
			}
		if (isEmpty && x != _snake.head.x && y != _snake.head.y) {
			_fruit = sf::Vector2u(x, y);
			break;
		}
	}
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

	auto offset { offsets.at(_snake.newDirection) };
	return sf::Vector2u(_snake.head.x + offset.x, _snake.head.y + offset.y); 
}

void GameState::changeDirection(Direction newDirection) {
	switch (newDirection) {
		case Direction::DOWN:
		case Direction::UP:
			if (_snake.direction == Direction::LEFT || 
			    _snake.direction == Direction::RIGHT)
				_snake.newDirection = newDirection;
			break;
		case Direction::LEFT:
		case Direction::RIGHT:
			if (_snake.direction == Direction::UP ||
			    _snake.direction == Direction::DOWN)
				_snake.newDirection = newDirection;
			break;
	}
}

void GameState::makeStep() {
	if (isGameOver()) {
		_data->machine.addState(StateRef(new GameOverState(_data, _scoreValue)));
		return;
	}

	auto newHead { getNewHead() };
	_snake.points.push_front(_snake.head);
	_snake.head = newHead;
	_snake.direction = _snake.newDirection;
	if (_snake.head.x == _fruit.x && _snake.head.y == _fruit.y) {
		generateFruit();
		_scoreValue++;
	}
	else
		_snake.points.pop_back();
	
}

void GameState::updateDeltaTime() {
	_deltaTime.time = _deltaTime.clock.getElapsedTime().asSeconds();
}

void GameState::drawFruit() {
	auto [x, y] { getBlockPosition(_fruit.x, _fruit.y) };
	_fruitCell.setPosition(x, y);
	_data->window.draw(_fruitCell);
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
			
			auto [x, y] { getBlockPosition(i, j) };
			rect.setPosition(x, y);
			_data->window.draw(rect);
		}
}

void GameState::drawSnake() {
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Blue);
	rect.setSize(sf::Vector2f(_output.blockSize, _output.blockSize));
	
	auto [x, y] { getBlockPosition(_snake.head.x, _snake.head.y) };
	rect.setPosition(sf::Vector2f(x, y));
	_data->window.draw(rect);
	
	for (auto &point : _snake.points) {
		auto [x, y] { getBlockPosition(point.x, point.y) };
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
	_snake.newDirection = _snake.direction;

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

	_fruitCell.setFillColor(sf::Color(200, 0, 0));
	_fruitCell.setSize(sf::Vector2f(_output.blockSize, _output.blockSize));

	generateFruit();
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
				else if (event.key.code == sf::Keyboard::Escape)
					_data->machine.addState(StateRef(new PauseState(_data)), false);
				break;
			default:
				break;
		}
	}
}

void GameState::update(float dt) {
	updateDeltaTime();

	if (_deltaTime.time > Settings::Latency) {
		makeStep();
		_deltaTime.clock.restart();
	}
}

void GameState::draw(float dt) {
	_data->window.clear();

	// Drawing items
	drawField();
	drawScore();

	drawFruit();
	drawSnake();

	_data->window.display();
}
