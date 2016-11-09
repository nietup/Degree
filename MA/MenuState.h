#pragma once
#include "State.h"
#include "StateMachine.h"

class MenuState : public State {
public:
	MenuState(StateMachine*);

	void HandleInput();
	void Update();
	void Render();
	void Show();
	void Hide();
};

