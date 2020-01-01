#include "game.h"

int main() {
	Game game(sf::VideoMode(200, 200), "Tetris");
	game.run();
	return 0;
}
