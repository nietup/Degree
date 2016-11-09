#pragma once
#include "State.h"

class MenuState : public State {
public:
	MenuState(WindowPack*);

	void HandleInput();
	void Update();
	void Render();
	void Show();
	void Hide();
};

