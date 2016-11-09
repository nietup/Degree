#pragma once
#include "State.h"
#include "WindowPack.h"

class StateMachine {
public:
	void HandleInput();
	void Update();
	void Render();
	void ChangeState(State*);

private:
	State * CurrentState;
	WindowPack * Window;
};
