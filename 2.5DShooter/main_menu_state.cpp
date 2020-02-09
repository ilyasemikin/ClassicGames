#include "main_menu_state.h"

MainMenuState::MainMenuState(GameDataRef data) : _data(data) {

}

MainMenuState::~MainMenuState() {

}

void MainMenuState::init() {

}

void MainMenuState::handleInput() {
	sf::Event event;

	while (_data->window.pollEvent(event))
		switch (event.type) {
			case sf::Event::Closed:
				_data->window.close();
				break;
			default:
				break;
		}
}

void MainMenuState::draw(float dt) {
	_data->window.clear();

	_data->window.display();
}

void MainMenuState::update(float dt) {

}
