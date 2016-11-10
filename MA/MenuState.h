#pragma once
#include "State.h"
#include "StateMachine.h"
#include "NewModelState.h"

class MenuState : public State {
public:
	MenuState(StateMachine*);

	void HandleInput() override;
};

