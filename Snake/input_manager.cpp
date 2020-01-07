#include "input_manager.h"

bool InputManager::isSpriteClicked(const sf::Sprite &sprite, const sf::Mouse::Button &button, const sf::RenderWindow &window) {
	if (sf::Mouse::isButtonPressed(button)) {
		auto pos { sprite.getPosition() };
		auto x { pos.x };
		auto y { pos.y };
		auto bounds { sprite.getGlobalBounds() };
 	      	auto width { bounds.width };
		auto height { bounds.height };

		sf::IntRect rect(x, y, width, height);

		return rect.contains(sf::Mouse::getPosition(window));
	}

	return false;
}

sf::Vector2i InputManager::getMousePosition(sf::RenderWindow &target) {
	return sf::Mouse::getPosition(target);
}
