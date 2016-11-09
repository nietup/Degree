#include "StateMachine.h"
#include <iostream>

StateMachine::StateMachine(int WindowWidth, int WindowHeight, int Rate, string Title) {
	Window = new WindowPack(WindowWidth, WindowHeight, Rate, Title);
}

void StateMachine::HandleInput() {
	CurrentState->HandleInput();
}

void StateMachine::Update() {
	Window->HandleEvent();
	CurrentState->Update();
}

void StateMachine::Render() {
	Window->Render();
	CurrentState->Render();
}

void StateMachine::ChangeState(State* NewState) {
	if (NULL != CurrentState) {
		CurrentState->Hide();
		delete CurrentState;
	}
	CurrentState = NewState;
	CurrentState->Show();
}