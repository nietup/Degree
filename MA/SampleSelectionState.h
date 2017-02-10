#pragma once
#include "State.h"
#include "Model.h"
#include "StateMachine.h"

class SampleSelectionState : public State {
public:
	SampleSelectionState(StateMachine*, Model *);
	~SampleSelectionState();

    /*Checks given folders
     * iterates over given files
     * runs segmentation for the learning algorithm
     */
	void PrepareLearningData();
    void SetPathStrings();

private:
	Model * LearnedModel;
	tgui::EditBox::Ptr PositiveFolder, NegativeFolder;
    string PositivePath, NegativePath;
};

