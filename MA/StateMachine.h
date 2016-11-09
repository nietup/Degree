#pragma once
#include "WindowPack.h"
#include "State.h"

class StateMachine {
public:
	StateMachine(int, int, int, string);

	void HandleInput();
	void Update();
	void Render();
	void ChangeState(State*);

	WindowPack * GetWindow() { return Window; };

private:
	State * CurrentState;
	WindowPack * Window;
};
