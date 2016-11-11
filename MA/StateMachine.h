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
	bool ChangeState(std::string);
	void Close();

	WindowPack * GetWindow() { return Window; };
	bool IsRunning() { return Running; };

private:
	State * CurrentState;
	WindowPack * Window;
	std::vector<State *> * States;
	bool Running;
};
