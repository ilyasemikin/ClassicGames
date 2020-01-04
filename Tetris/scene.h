#pragma once

#include <SFML/Graphics.hpp>

class Scene {
protected:

public:
	virtual bool isOver() = 0;

	virtual void handleKey(sf::Event::KeyEvent) = 0;
	virtual void update() = 0;
	virtual void display(sf::RenderWindow &target) = 0;
};
