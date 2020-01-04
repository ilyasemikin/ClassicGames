#include "game_scene.h"
#include "game.h"

Game::Game() : Game(sf::VideoMode(800, 600), "window") {

}

Game::Game(sf::VideoMode vm, std::string title) : 
	window(vm, title, sf::Style::Close | sf::Style::Titlebar) 
{
	initScenes();
}

Game::~Game() {

}

void Game::initScenes() {
	scenes.push(std::make_unique<GameScene>(10, 5));
}

void Game::updateDeltaTime() {
	deltaTime = deltaTimeClock.restart().asSeconds();
}

void Game::updateSFEvent() {
	while (window.pollEvent(sfEvent))
		switch (sfEvent.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (!scenes.empty())
					scenes.top()->handleKey(sfEvent.key);
				break;
			default:
				break;
		}
}

void Game::update() {
	updateDeltaTime();
	updateSFEvent();

	if (!scenes.empty()) {
		scenes.top()->update();

		if (scenes.top()->isOver())
			scenes.pop();
	}
	else
		window.close();
}

void Game::display() {
	window.clear();

	// Display items
	if (!scenes.empty())
		scenes.top()->display(window);

	window.display();
}

void Game::run() {
	while (window.isOpen()) {
		update();
		display();
	}
}
