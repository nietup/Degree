//
// Created by nietup on 04.01.17.
//

#ifndef DEGREE_MODELSELECTIONSTATE_H
#define DEGREE_MODELSELECTIONSTATE_H

#include "StateMachine.h"
#include "Model.h"
#include "WindowPack.h"

class ModelSelectionState : public State {
public:
    ModelSelectionState(StateMachine*);

private:
    void LoadModel();

    Model * chosenModel;
    std::string modelName, path;
    tgui::EditBox::Ptr modelNamePtr, pathPtr;
};


#endif //DEGREE_MODELSELECTIONSTATE_H
