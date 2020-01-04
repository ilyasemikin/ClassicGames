#include <memory>
#include <stack>
#include <string>
#include <SFML/Graphics.hpp>
#include "scene.h"

class Game {
private:
	sf::RenderWindow window;
	sf::Event sfEvent;
	sf::Clock deltaTimeClock;
	float deltaTime;
	std::stack<std::unique_ptr<Scene>> scenes;

	void initScenes();

	void updateDeltaTime();
	void updateSFEvent();
	void update();
	void display();
public:
	Game();
	Game(sf::VideoMode vm, std::string title);
	virtual ~Game();
	void run();
};
