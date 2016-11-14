#pragma once
#include "State.h"
#include "Model.h"
#include "StateMachine.h"

class SampleSelectionState : public State {
public:
	SampleSelectionState(StateMachine*, Model *);
	~SampleSelectionState();

private:
	Model * LearnedModel;
	tgui::EditBox::Ptr PositiveFolder, NegativeFolder;
};

