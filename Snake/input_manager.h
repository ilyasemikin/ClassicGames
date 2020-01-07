#pragma once

#include <SFML/Graphics.hpp>

class InputManager {
public:
	InputManager() {}
	virtual ~InputManager() {}
	
	bool isSpriteClicked(const sf::Sprite &sprite, const sf::Mouse::Button &button, const sf::RenderWindow &window);
	sf::Vector2i getMousePosition(sf::RenderWindow &target);
};
