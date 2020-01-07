#include "main_menu_state.h"
#include "state.h"
#include "game.h"

Game::Game(size_t width, size_t height, const std::string &title) {
	_data = std::make_shared<GameData>();
	_data->window.create(
		sf::VideoMode(width, height), 
		title,
		sf::Style::Titlebar | sf::Style::Close
	);

	_data->machine.addState(StateRef(new MainMenuState(_data)));

	deltaTime = 1.0f / 60;
}

void Game::run() {
	float newTime, frameTime, interpolation;

	float currentTime { _clock.getElapsedTime().asSeconds() };
	float accumulator { 0 };

	while (_data->window.isOpen()) {
		_data->machine.processStateChanges();

		newTime = _clock.getElapsedTime().asSeconds();
		frameTime = newTime - currentTime;

		if (frameTime > 0.25)
			frameTime = 0.25;

		currentTime = newTime;
		accumulator += frameTime;
	
		while (accumulator >= deltaTime) {
			_data->machine.getActiveState()->handleInput();
			_data->machine.getActiveState()->update(deltaTime);

			accumulator -= deltaTime;
		}

		interpolation = accumulator / deltaTime;
		_data->machine.getActiveState()->draw(interpolation);
	}
}
