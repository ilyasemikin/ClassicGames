#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "scene.h"
#include "point.h"
#include "rectangle.h"

class GameScene : public Scene {
private:
	std::string blockTexturePath;
	size_t blockTextureSize;
	std::vector<sf::Texture> blockTextures;
	
	std::string scoreFontPath;
	sf::Font scoreFont;

	size_t fieldLines;
	size_t fieldColumns;
	std::vector<std::vector<Point>> field;

	// Score part of screen
	float scorePart;

	bool isSceneOver;

	void initTextures();
	void initFonts();

	Rectangle getScoreWindowPlace(const sf::Vector2u &wndSize);
	Rectangle getGameWindowPlace(const sf::Vector2u &wndSize);
	float getBlockSize(const Rectangle &wndPlace);
public:
	GameScene(size_t m, size_t n);
	virtual ~GameScene();

	bool isOver() override;

	void handleKey(sf::Event::KeyEvent) override;
	void update() override;
	void display(sf::RenderWindow &target) override;
};
