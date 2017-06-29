#include "StateMachine.h"

StateMachine::StateMachine(int WindowWidth, int WindowHeight, int Rate, string Title) {
	Window = new WindowPack(WindowWidth, WindowHeight, Rate, Title);
	States = new std::vector<State *>();
	Running = true;
	CurrentState = nullptr;
}

StateMachine::~StateMachine() {
	Running = false;
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
	if (nullptr != CurrentState) {
		CurrentState->Hide();
	}
	States->push_back(NewState);
	CurrentState = NewState;
	CurrentState->Show();
}

bool StateMachine::ChangeState(std::string Name) {
	for (auto &State : *States) {
		if (State->GetName() == Name) {
			if (NULL != CurrentState) {
				CurrentState->Hide();
			}
			CurrentState = State;
			CurrentState->Show();
			return true;
		}
	}
	return false;
}

void StateMachine::Close() {
	Window->Close();
	Running = false;
}
