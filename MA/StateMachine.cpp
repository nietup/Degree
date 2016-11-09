#include "StateMachine.h"


void StateMachine::HandleInput() {
	CurrentState->HandleInput();
}

void StateMachine::Update() {
	CurrentState->Update();
}

void StateMachine::Render() {
	CurrentState->Render();
}

void StateMachine::ChangeState(State* NewState) {
	CurrentState->Hide();
	delete CurrentState;
	CurrentState = NewState;
	CurrentState->Show();
}