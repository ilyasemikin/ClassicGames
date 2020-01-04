#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "scene.h"
#include "point_state.h"
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
	std::vector<std::vector<PointState>> field;

	std::vector<std::vector<std::vector<PointState>>> figures;
	struct {
		std::vector<std::vector<PointState>> points;
		size_t x;
		size_t y;
		size_t width;
		size_t height;
		size_t texture;
	} figure;

	enum Direction : int { 
		LEFT, RIGHT, DOWN
	};

	// Score part of screen
	float scorePart;

	bool isSceneOver;

	void initTextures();
	void initFonts();
	void initFigures();

	void generateFigure();
	bool thereIsBarrier(Direction dir);
	void moveFigure(Direction dir);
	void fixFigure();
	
	void clearFieldLines();

	Rectangle getScoreWindowPlace(const sf::Vector2u &wndSize);
	Rectangle getGameWindowPlace(const sf::Vector2u &wndSize);
	float getBlockSize(const Rectangle &wndPlace);

	void printDebugInfo();
public:
	GameScene(size_t m, size_t n);
	virtual ~GameScene();

	bool isOver() override;

	void handleKey(sf::Event::KeyEvent) override;
	void update() override;
	void display(sf::RenderWindow &target) override;
};
