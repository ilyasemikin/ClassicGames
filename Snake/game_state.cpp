#include "game_state.h"

GameState::GameState(GameDataRef data, size_t width, size_t height) : _data(data) {
	_field.width = width;
	_field.height = height;
}

GameState::~GameState() {

}

Rectangle GameState::getScoreRect() {
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

float GameState::getBlockSize(const Rectangle &rect) {
	auto widthRatio { 
		static_cast<float>(_field.width) / rect.width 
	};
	auto heightRation {
		static_cast<float>(_field.height) / rect.height
	};
	if (widthRatio < heightRation)
		return static_cast<float>(rect.height) / _field.height;
	else
		return static_cast<float>(rect.width) / _field.width;
}

void GameState::init() {
	_data->assets.loadFont("RetroVilleNC", "Fonts/RetroVilleNC-Regular.ttf");

	_score.setFont(_data->assets.getFont("RetroVilleNC"));
	_score.setFillColor(sf::Color::White);
	_score.setString("Score:");
	auto scoreSpaceRect { getScoreRect() };
	_score.setCharacterSize(scoreSpaceRect.height * 0.95);

	auto mainSpaceRect { getMainRect() };
	auto blockSize( getBlockSize(mainSpaceRect) );
	_fieldBoundary.setFillColor(sf::Color(0, 64, 0));
	_fieldBoundary.setSize(sf::Vector2f(blockSize, blockSize));

	_oddEmptyCell.setFillColor(sf::Color(0, 128, 0));
	_oddEmptyCell.setSize(sf::Vector2f(blockSize, blockSize));

	_evenEmptyCell.setFillColor(sf::Color(0, 192, 0));
	_evenEmptyCell.setSize(sf::Vector2f(blockSize, blockSize));
}

void GameState::handleInput() {
	sf::Event event;

	while (_data->window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				_data->window.close();
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

	_data->window.draw(_score);
	auto isBoundaryCell {
		[&](size_t x, size_t y) {
			return x == 0 || x == _field.width - 1 || y == 0 || y == _field.height - 1;
		}
	};
	auto wndSize { getMainRect() };
	auto blockSize { getBlockSize(wndSize) };
	for (size_t i = 0; i < _field.width; i++)
		for (size_t j = 0; j < _field.height; j++) {
			sf::RectangleShape rect;
			if (isBoundaryCell(i, j))
				rect = _fieldBoundary;
			else if ((i + j) % 2 != 0)
				rect = _oddEmptyCell;
			else
				rect = _evenEmptyCell;
			rect.setPosition(
				wndSize.x + i * blockSize,
				wndSize.y + j * blockSize
			);
			_data->window.draw(rect);
		}
			
	_data->window.display();
}
