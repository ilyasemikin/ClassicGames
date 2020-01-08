#include "main_menu_state.h"
#include "pause_state.h"

PauseState::PauseState(GameDataRef data) : _data(data) {

}

PauseState::~PauseState() {

}

void PauseState::init() {
    _data->assets.loadFont("RetroVilleNC", "Fonts/RetroVilleNC-Regular.ttf");

    auto wndSize { _data->window.getSize() };

	_pauseText.setFont(_data->assets.getFont("RetroVilleNC"));
	_pauseText.setFillColor(sf::Color::Red);
	_pauseText.setString("PAUSE");
	_pauseText.setCharacterSize(32);
	auto pauseTextSize { _pauseText.getGlobalBounds() };
	_pauseText.setPosition(
		wndSize.x / 2 - pauseTextSize.width / 2,
		wndSize.y / 2 - pauseTextSize.height / 2 - 60
	);

    _continueButton.setFont(_data->assets.getFont("RetroVilleNC"));
	_continueButton.setFillColor(sf::Color::White);
	_continueButton.setString("Continue");
	_continueButton.setCharacterSize(24);
	auto continueButtonSize { _continueButton.getGlobalBounds() };
	_continueButton.setPosition(
		wndSize.x / 2 - continueButtonSize.width / 2,
		wndSize.y / 2 - continueButtonSize.height / 2 - 20
	);

    _quitButton.setFont(_data->assets.getFont("RetroVilleNC"));
	_quitButton.setFillColor(sf::Color::White);
	_quitButton.setString("Man menu");
	_quitButton.setCharacterSize(24);
	auto quitButtonSize { _quitButton.getGlobalBounds() };
	_quitButton.setPosition(
		wndSize.x / 2 - quitButtonSize.width / 2,
		wndSize.y / 2 - quitButtonSize.height / 2 + 20
	);
}

void PauseState::handleInput() {
    sf::Event event;

	while (_data->window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				_data->window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
                    _data->machine.removeState();
				break;
			default:
				break;
		}
	}

	if (_data->input.isClicked(_continueButton, sf::Mouse::Left, _data->window))
		_data->machine.removeState();
	else if (_data->input.isClicked(_quitButton, sf::Mouse::Left, _data->window)) {
		_data->machine.removeState();
		_data->machine.addState(StateRef(new MainMenuState(_data)));
	}
}

void PauseState::update(float dt) {

}

void PauseState::draw(float dt) {
    _data->window.clear();

	_data->window.draw(_pauseText);
    _data->window.draw(_continueButton);
    _data->window.draw(_quitButton);

    _data->window.display();
}