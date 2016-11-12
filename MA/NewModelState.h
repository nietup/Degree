#pragma once
#include <iostream>
#include "State.h"
#include "StateMachine.h"
#include "Primitive.h"
#include "Abstract.h"
#include "Solid.h"

class MenuState;

class NewModelState : public State {
public:
	NewModelState(StateMachine*);

	void DisplayRelationships(int);
	void DeleteRelationships();

	void Show() override;

	void printuj(int i) {
		std::cout << i;
	};

private:
	std::string ModelName;
	int ModelLevel;
	std::vector<int> * ModelRelationships;
	std::vector<tgui::Widget::Ptr> * DisplayedRelationships;
};

