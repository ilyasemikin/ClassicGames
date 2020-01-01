#include "game.h"

Game::Game() : Game(sf::VideoMode(800, 600), "window") {

}

Game::Game(sf::VideoMode vm, std::string title) : window(vm, title) {

}

Game::~Game() {

}

void Game::updateSFEvent() {
	while (window.pollEvent(sfEvent))
		if (sfEvent.type == sf::Event::Closed)
			window.close();
}

void Game::update() {

}

void Game::display() {
	window.clear();
	window.display();
}

void Game::run() {
	while (window.isOpen()) {
		updateSFEvent();
		update();
		display();
	}
}
