#pragma once

#include <SFML/Graphics.hpp>

class InputManager {
public:
	InputManager() {}
	virtual ~InputManager() {}
	
	bool isClicked(const sf::Sprite &sprite, const sf::Mouse::Button &button, const sf::RenderWindow &window);
	bool isClicked(const sf::Text &text, const sf::Mouse::Button &button, const sf::RenderWindow &window);
	bool isClicked(const sf::IntRect &rect, const sf::Mouse::Button &button, const sf::RenderWindow &window);
	sf::Vector2i getMousePosition(sf::RenderWindow &target);
};
