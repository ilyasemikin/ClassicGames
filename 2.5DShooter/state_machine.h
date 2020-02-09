#pragma once

#include <stack>
#include <memory>
#include "state.h"

class StateMachine {
private:
	StateRef _newState;

	bool _isAdding;
	bool _isReplacing;
	bool _isRemoving;

	std::stack<StateRef> _states;
public:
	StateMachine() {}
	virtual ~StateMachine() {}

	void addState(StateRef newState, bool isReplacing = true);
	void removeState();

	void processStateChanges();

	bool areStatesAvailable();

	StateRef &getActiveState();
};
