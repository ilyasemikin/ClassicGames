#include <iostream>
#include <algorithm>
#include <list>
#include "png_image.h"
#include "game_scene.h"

GameScene::GameScene(size_t m, size_t n) : 
	blockTexturePath("Textures/blocks.png"),
	scoreFontPath("Fonts/Roboto-Black.ttf"),
	fieldLines(m),
	fieldColumns(n),
	field(m, std::vector<PointState>(n, PointState())),
	scorePart(0.05),
	isSceneOver(false)
{
	initTextures();
	initFonts();
	initFigures();

	generateFigure();
}

GameScene::~GameScene() {

}

void GameScene::initTextures() {
	auto textureSize { PNGImage::getSize(blockTexturePath) };
	
	if (textureSize.width == 0 || textureSize.height == 0)
		throw "";

	if (textureSize.width < textureSize.height 
	    && textureSize.width % textureSize.height != 0)
		throw "";

	blockTextureSize = textureSize.height;
	size_t count { textureSize.width / blockTextureSize };
	blockTextures.resize(count);

	for (size_t i = 0; i < count; i++) {
		auto isLoaded {
			blockTextures[i].loadFromFile(
				blockTexturePath,
				sf::IntRect(
					i * blockTextureSize, 
					0, 
					blockTextureSize,
					blockTextureSize
				)
			)
		};
		if (!isLoaded) {
			std::cerr << "Can't load texture" << std::endl;
			throw "";
		}
	}
}

void GameScene::initFonts() {
	if (!scoreFont.loadFromFile(scoreFontPath)) {
		std::cerr << "" << std::endl;
		throw "";
	}
}

void GameScene::initFigures() {
	figures.push_back(
		{ 
			{ 0, 1, 0 },
			{ 1, 1, 1 }
		}
	);
}

void GameScene::generateFigure() {
	figure.points = figures[0];
	figure.x = 0;
	figure.y = 0;
	figure.width = figure.points[0].size();
	figure.height = figure.points.size();
	figure.texture = 0;
}

bool GameScene::thereIsBarrier(Direction dir) {
	switch (dir) {
		case Direction::LEFT:
			if (figure.x == 0)
				return true;
			for (size_t i = 0; i < figure.height; i++) {
				size_t j = 0;
				while (!figure.points[i][j].isFilled)
					j++;
				if (field[figure.y + i][figure.x + j - 1].isFilled)
					return true;
			}
			return false;
		case Direction::RIGHT:
			if (figure.x + figure.width == fieldColumns)
				return true;
			for (size_t i = 0; i < figure.height; i++) {
				size_t j = figure.width - 1;
				while (!figure.points[i][j].isFilled)
					j--;
				if (field[figure.y + i][figure.x + j + 1].isFilled)
					return true;
			}
			return false;
		case Direction::DOWN:
			if (figure.y + figure.height == fieldLines)
				return true;
			for (size_t j = 0; j < figure.width; j++) {
				size_t i = figure.height - 1;
				while (!figure.points[i][j].isFilled)
					i--;
				if (field[figure.y + i + 1][figure.x + j].isFilled)
					return true;
			}
			return false;
	}
	return true;
}

void GameScene::moveFigure(Direction dir) {
	switch (dir) {
		case Direction::LEFT:
			if (!thereIsBarrier(dir))
				figure.x--;
			break;
		case Direction::RIGHT:
			if (!thereIsBarrier(dir))
				figure.x++;
			break;
		case Direction::DOWN:
			if (!thereIsBarrier(dir))
				figure.y++;
			else {
				fixFigure();
				clearFieldLines();
				generateFigure();
			}
			break;
	}
}

void GameScene::fixFigure() {
	for (size_t i = 0; i < figure.height; i++)
		for (size_t j = 0; j < figure.width; j++) {
			auto jField { figure.x + j };
			auto iField { figure.y + i };
			if (figure.points[i][j].isFilled) {
				field[iField][jField].isFilled = true;
				field[iField][jField].texture = 2;
			}
		}
}

void GameScene::clearFieldLines() {
	auto pred { [](PointState x) { return x.isFilled; } };
	std::list<size_t> deleted;
	for (size_t i = 0; i < fieldLines; i++) {
		auto isCompletelyFilled { std::all_of(field[i].begin(), field[i].end(), pred) };
		if (isCompletelyFilled) {
			deleted.push_back(i);
			std::fill(field[i].begin(), field[i].end(), PointState());
		}
	}

	for (auto &line : deleted) {
		for (size_t i = line; i > 0; i--)
			field[i] = field[i - 1];
		std::fill(field[0].begin(), field[0].end(), PointState());
	}
}

Rectangle GameScene::getScoreWindowPlace(const sf::Vector2u &wndSize) {
	Rectangle ret;
	ret.x = 0;
	ret.y = 0;
	ret.width = wndSize.x;
	ret.height = wndSize.y * scorePart;
	return ret;
}

Rectangle GameScene::getGameWindowPlace(const sf::Vector2u &wndSize) {
	Rectangle ret;
	ret.x = 0;
	ret.y = wndSize.y * scorePart + 1;
	ret.width = wndSize.x;
	ret.height = wndSize.y - ret.y;
	return ret;
}

float GameScene::getBlockSize(const Rectangle &wndPlace) {
	auto widthRatio { 
		static_cast<float>(fieldColumns * blockTextureSize) / wndPlace.width 
	};
	auto heightRation { 
		static_cast<float>(fieldLines * blockTextureSize) / wndPlace.height 
	};
	if (widthRatio < heightRation)
		return static_cast<float>(wndPlace.height) / fieldLines;
	else
		return static_cast<float>(wndPlace.width) / fieldColumns;
}

void GameScene::printDebugInfo() {
	std::cout << "Field:" << std::endl;
	std::cout << "Count lines = " << fieldLines << "; count columns = " << fieldColumns << std::endl;
	for (size_t i = 0; i < fieldLines; i++) {
		for (size_t j = 0; j < fieldColumns; j++) {
			if (figure.x <= j && j < figure.x + figure.width &&
			    figure.y <= i && i < figure.y + figure.height &&
			    figure.points[i - figure.y][j - figure.x].isFilled)
				std::cout << 'F';
			else
				std::cout << field[i][j].isFilled;
		}
		std::cout << std::endl;
	}
	std::cout << "Figure:" << std::endl;
	std::cout << "X = " << figure.x << "; y = " << figure.y << std::endl;
	std::cout << "Width = " << figure.width << "; height = " << figure.height << std::endl;
	for (size_t i = 0; i < figure.height; i++) {
		for (size_t j = 0; j < figure.width; j++)
			std::cout << figure.points[i][j].isFilled;
		std::cout << std::endl;
	}
}

bool GameScene::isOver() {
	return isSceneOver;
}

void GameScene::handleKey(sf::Event::KeyEvent event) {
	switch (event.code) {
		case sf::Keyboard::Key::Up:
			break;
		case sf::Keyboard::Key::Down:
			break;
		case sf::Keyboard::Key::Right:
			moveFigure(Direction::RIGHT);
			break;
		case sf::Keyboard::Key::Left:
			moveFigure(Direction::LEFT);
			break;
		default:
			break;
	}
	printDebugInfo();
}

void GameScene::update() {
	static sf::Clock deltaClock;
	const float latency = 0.2;
	if (deltaClock.getElapsedTime().asSeconds() > latency) {
		moveFigure(Direction::DOWN);
		deltaClock.restart();
	}
}

void GameScene::display(sf::RenderWindow &target) {
	auto scoreWndPlace { getScoreWindowPlace(target.getSize()) };
	auto gameWndPlace { getGameWindowPlace(target.getSize()) };

	sf::Text text;
	const float scoreTextScale = 0.85;
	text.setFont(scoreFont);
	text.setCharacterSize(scoreWndPlace.height * scoreTextScale);
	text.setString("Score");
	text.setFillColor(sf::Color::White);

	target.draw(text);

	auto blockSize { getBlockSize(gameWndPlace) };
	auto blockScale { blockSize / blockTextureSize };
	sf::Sprite sprite;
	sprite.setScale(blockScale, blockScale);

	for (size_t i = 0; i < figure.height; i++)
		for (size_t j = 0; j < figure.width; j++)
			if (figure.points[i][j].isFilled) {
				sprite.setPosition(
					gameWndPlace.x + (figure.x + j) * blockSize,
					gameWndPlace.y + (figure.y + i) * blockSize
				);
				sprite.setTexture(blockTextures[figure.texture]);
				target.draw(sprite);
			}

	for (size_t i = 0; i < fieldLines; i++)
		for (size_t j = 0; j < fieldColumns; j++)
			if (field[i][j].isFilled) {
				sprite.setPosition(
					gameWndPlace.x + j * blockSize,
					gameWndPlace.y + i * blockSize
				);
				sprite.setTexture(blockTextures[field[i][j].texture]);
				target.draw(sprite);
			}
}
