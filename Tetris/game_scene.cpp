#include <iostream>
#include "png_image.h"
#include "game_scene.h"

GameScene::GameScene(size_t m, size_t n) : 
	blockTexturePath("Textures/blocks.png"),
	scoreFontPath("Fonts/Roboto-Black.ttf"),
	fieldLines(m),
	fieldColumns(n),
	field(m, std::vector<Point>(n, Point())),
	scorePart(0.05),
	isSceneOver(false)
{
	initTextures();
	initFonts();
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
			break;
		case sf::Keyboard::Key::Left:
			break;
		default:
			break;
	}
}

void GameScene::update() {

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
