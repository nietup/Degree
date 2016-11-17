#pragma once
#include <iostream>
#include <fstream>
#include <windows.h>
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
	std::string ModelName, SaveFolder;
	int ModelLevel;
	std::vector<int> * ModelRelationships;
	std::vector<tgui::Widget::Ptr> * DisplayedRelationships;
	tgui::EditBox::Ptr ModelNamePtr, SaveFolderPtr;
	Model * NewModel;

	std::wstring s2ws(const std::string& s);
};

