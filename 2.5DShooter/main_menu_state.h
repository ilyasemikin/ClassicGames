#pragma once

#include "game_data.h"
#include "state.h"

class MainMenuState : public State {
private:
	GameDataRef _data;
public:
	MainMenuState(GameDataRef data);
	~MainMenuState();

	void init() override;

	void handleInput() override;
	void update(float dt) override;
	void draw(float dt) override;
};
