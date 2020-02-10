#include "game.h"
#include "game_state.h"
#include "settings.h"

int main() {
	Game game(Settings::WindowWidth, Settings::WindowHeight, Settings::WindowTitle);
	game.setStartState<GameState>();
	
	game.run();

	return 0;
}
