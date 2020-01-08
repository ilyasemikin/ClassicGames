#include "game_state.h"
#include "main_menu_state.h"
#include "settings.h"
#include "game_over_state.h"

GameOverState::GameOverState(GameDataRef data, size_t score) : 
    _data(data),
    _score(score)
{

}

GameOverState::~GameOverState() {

}

void GameOverState::init() {
    _data->assets.loadFont("RetroVilleNC", "Fonts/RetroVilleNC-Regular.ttf");

    auto wndSize { _data->window.getSize() };

    _gameOverText.setFont(_data->assets.getFont("RetroVilleNC"));
    _gameOverText.setFillColor(sf::Color::Red);
    _gameOverText.setString("GAME OVER");
    _gameOverText.setCharacterSize(32);
    auto gameOverSize { _gameOverText.getGlobalBounds() };
    _gameOverText.setPosition(
        wndSize.x / 2 - gameOverSize.width / 2,
        wndSize.y / 2 - gameOverSize.height / 2 - 90
    );

    _scoreText.setFont(_data->assets.getFont("RetroVilleNC"));
    _scoreText.setFillColor(sf::Color::Green);
    _scoreText.setString("Score: " + std::to_string(_score));
    _scoreText.setCharacterSize(24);
    auto scoreSize { _scoreText.getGlobalBounds() };
    _scoreText.setPosition(
        wndSize.x / 2 - scoreSize.width / 2,
        wndSize.y / 2 - scoreSize.height / 2 - 60
    );

    _retryButton.setFont(_data->assets.getFont("RetroVilleNC"));
	_retryButton.setFillColor(sf::Color::White);
	_retryButton.setString("Retry");
	_retryButton.setCharacterSize(24);
	auto retryButtonSize { _retryButton.getGlobalBounds() };
	_retryButton.setPosition(
		wndSize.x / 2 - retryButtonSize.width / 2,
		wndSize.y / 2 - retryButtonSize.height / 2 - 20
	);

    _quitButton.setFont(_data->assets.getFont("RetroVilleNC"));
	_quitButton.setFillColor(sf::Color::White);
	_quitButton.setString("Quit");
	_quitButton.setCharacterSize(24);
	auto quitButtonSize { _quitButton.getGlobalBounds() };
	_quitButton.setPosition(
		wndSize.x / 2 - quitButtonSize.width / 2,
		wndSize.y / 2 - quitButtonSize.height / 2 + 20
	);
}

void GameOverState::handleInput() {
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

	if (_data->input.isClicked(_retryButton, sf::Mouse::Left, _data->window))
		_data->machine.addState(StateRef(new GameState(
            _data, Settings::FieldWidth, Settings::FieldHeight
        )));
	else if (_data->input.isClicked(_quitButton, sf::Mouse::Left, _data->window))
		_data->machine.addState(StateRef(new MainMenuState(_data)));
}

void GameOverState::update(float dt) {

}

void GameOverState::draw(float dt) {
    _data->window.clear();

    _data->window.draw(_gameOverText);
    _data->window.draw(_scoreText);
    _data->window.draw(_retryButton);
    _data->window.draw(_quitButton);

    _data->window.display();
}