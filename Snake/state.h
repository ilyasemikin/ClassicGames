#pragma once

#include <memory>

class State {
protected:
	
public:
	virtual void init() = 0;
	
	virtual void handleInput() = 0;
	virtual void update(float dt) = 0;
	virtual void draw(float dt) = 0;

	virtual void pause() {}
	virtual void resume() {}
};

using StateRef = std::unique_ptr<State>;
