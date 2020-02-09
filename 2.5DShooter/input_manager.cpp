#include "input_manager.h"

bool InputManager::isClicked(const sf::Sprite &sprite, const sf::Mouse::Button &button, const sf::RenderWindow &window) {
	auto pos { sprite.getPosition() };
	auto x { pos.x };
	auto y { pos.y };
	auto bounds { sprite.getGlobalBounds() };
 	auto width { bounds.width };
	auto height { bounds.height };
	sf::IntRect rect(x, y, width, height);
	return rect.contains(sf::Mouse::getPosition(window));
}

bool InputManager::isClicked(const sf::Text &text, const sf::Mouse::Button &button, const sf::RenderWindow &window) {
	auto pos { text.getPosition() };
	auto x { pos.x };
	auto y { pos.y };
	auto bounds { text.getGlobalBounds() };
	auto width { bounds.width };
	auto height { bounds.height };
	sf::IntRect rect(x, y, width, height);
	return isClicked(rect, button, window);
}

bool InputManager::isClicked(const sf::IntRect &rect, const sf::Mouse::Button &button, const sf::RenderWindow &window) {
	if (sf::Mouse::isButtonPressed(button))
			return rect.contains(sf::Mouse::getPosition(window));
	return false;
}

sf::Vector2i InputManager::getMousePosition(sf::RenderWindow &target) {
	return sf::Mouse::getPosition(target);
}
