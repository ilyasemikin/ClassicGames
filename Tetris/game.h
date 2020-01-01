#include <string>
#include <SFML/Graphics.hpp>

class Game {
private:
	sf::RenderWindow window;
	sf::Event sfEvent;

	void updateSFEvent();
	void update();
	void display();
public:
	Game();
	Game(sf::VideoMode vm, std::string title);
	virtual ~Game();
	void run();
};
