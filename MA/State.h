#pragma once
#include <TGUI/TGUI.hpp>

class StateMachine;

class State {
public:
	virtual void HandleInput();
	virtual void Update();
	virtual void Render();
	virtual void Show();
	virtual void Hide();

	std::string GetName() { return Name; };

protected:
	StateMachine * Manager;
	std::vector<tgui::Widget::Ptr> * Widgets;
	std::string Name;
};
