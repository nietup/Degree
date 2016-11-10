#pragma once
#include "State.h"
#include "StateMachine.h"

class MenuState;

class NewModelState : public State {
public:
	NewModelState(StateMachine*);
};

