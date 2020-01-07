#include "game_state.h"
#include "state.h"
#include "main_menu_state.h"

MainMenuState::MainMenuState(GameDataRef data) : _data(data) {

}

MainMenuState::~MainMenuState() {

}

Rectangle MainMenuState::getTitleRect() {
	Rectangle ret;
	auto wndSize { _data->window.getSize() };
	ret.x = 0;
	ret.y = 0;
	ret.width = wndSize.x;
	ret.height = wndSize.y * 0.1;
	return ret;
}

Rectangle MainMenuState::getButtonsRect() {
	Rectangle ret;
	auto wndSize { _data->window.getSize() };
	ret.x = 0;
	ret.y = wndSize.y * 0.1 + 1;
	ret.width = wndSize.x;
	ret.height = wndSize.y - ret.y;
	return ret;
}

void MainMenuState::init() {
	_data->assets.loadFont("RetroVilleNC", "Fonts/RetroVilleNC-Regular.ttf");

	// Title initialization	
	_title.setFont(_data->assets.getFont("RetroVilleNC"));
	_title.setFillColor(sf::Color::White);
	_title.setString("Snake");
	_title.setCharacterSize(40);
	auto titleSpaceRect { getTitleRect() };
	auto titleSize { _title.getGlobalBounds() };
	auto x { titleSpaceRect.width / 2 - titleSize.width / 2 };
	auto y { titleSpaceRect.height / 2 };
	_title.setPosition(x, y);

	auto butsSpaceRect { getButtonsRect() };
	
	_playButton.setFont(_data->assets.getFont("RetroVilleNC"));
	_playButton.setFillColor(sf::Color::White);
	_playButton.setString("Play");
	_playButton.setCharacterSize(24);
	auto playButtonSize { _playButton.getGlobalBounds() };
	_playButton.setPosition(
		butsSpaceRect.width / 2 - playButtonSize.width / 2,
		butsSpaceRect.height / 2 - playButtonSize.height / 2 - 20
	);

	_quitButton.setFont(_data->assets.getFont("RetroVilleNC"));
	_quitButton.setFillColor(sf::Color::White);
	_quitButton.setString("Quit");
	_quitButton.setCharacterSize(24);
	auto quitButtonSize { _quitButton.getLocalBounds() };
	_quitButton.setPosition(
		butsSpaceRect.width / 2 - quitButtonSize.width / 2,
		butsSpaceRect.height / 2 - quitButtonSize.height / 2 + 20
	);
}

void MainMenuState::handleInput() {
	sf::Event event;

	while (_data->window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				_data->window.close();
				break;
			default:
				break;
		}
	}

	if (_data->input.isClicked(_playButton, sf::Mouse::Left, _data->window))
		_data->machine.addState(StateRef(new GameState(_data, 32, 60)));
	if (_data->input.isClicked(_quitButton, sf::Mouse::Left, _data->window))
		_data->window.close();
}

void MainMenuState::update(float dt) {

}

void MainMenuState::draw(float dt) {
	_data->window.clear();

	_data->window.draw(_title);
	_data->window.draw(_playButton);
	_data->window.draw(_quitButton);

	_data->window.display();
}
