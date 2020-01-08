#include "game.h"
#include "settings.h"

int main() {
	Game game(Settings::WindowWidth, Settings::WindowHeight, Settings::WindowTitle);
	game.run();
	return 0;
}
