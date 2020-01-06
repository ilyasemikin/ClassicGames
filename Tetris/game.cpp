#include <sstream>
#include "config_parser.h"
#include "game_scene.h"
#include "game.h"

Game::Game() : Game(sf::VideoMode(800, 600), "window") {

}

Game::Game(const std::string &configFile) {
	initScenes();
	loadConfigFromFile(configFile);
}

Game::Game(sf::VideoMode vm, std::string title) : 
	window(vm, title, sf::Style::Close | sf::Style::Titlebar) 
{
	initScenes();
}

Game::~Game() {

}

void Game::loadConfigFromFile(const std::string &path) {
	auto params {
		Config::getListParams(path, { "Width", "Height", "Title" })
	};
	auto width { Config::getParam<size_t>(params["Width"]) };
	auto height { Config::getParam<size_t>(params["Height"]) };
	window.create(sf::VideoMode(width, height, sf::Style::Close | sf::Style::Titlebar), params["Title"]);
}

void Game::initScenes() {
	scenes.push(std::make_unique<GameScene>("Configs/game", "Configs/figures"));
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
			case sf::Event::Resized:
			{
				sf::FloatRect visibleArea(0, 0, sfEvent.size.width, sfEvent.size.height);
				window.setView(sf::View(visibleArea));
				break;
			}
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
