#include "game.h"
#include "main_menu_state.h"
#include "settings.h"

int main() {
	Game game(Settings::WindowWidth, Settings::WindowHeight, Settings::WindowTitle);
	game.setStartState<MainMenuState>();
	
	game.run();

	return 0;
}
