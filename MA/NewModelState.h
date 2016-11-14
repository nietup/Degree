#pragma once
#include <iostream>
#include "State.h"
#include "StateMachine.h"
#include "Primitive.h"
#include "Abstract.h"
#include "Solid.h"
#include "SampleSelectionState.h"

class MenuState;

class NewModelState : public State {
public:
	NewModelState(StateMachine*);

	void DisplayRelationships(int);
	void DeleteRelationships();
	void CreateModel();

	void Show() override;
	void Hide() override;

	void Select(int);
	void Deselect(int);
	void DeselectAll();

private:
	std::string ModelName;
	int ModelLevel;
	std::vector<int> * ModelRelationships;
	std::vector<tgui::Widget::Ptr> * DisplayedRelationships;
	tgui::EditBox::Ptr ModelNamePtr;
	Model * NewModel;
};

