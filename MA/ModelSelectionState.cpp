//
// Created by nietup on 04.01.17.
//

#include "ModelSelectionState.h"

ModelSelectionState::ModelSelectionState(StateMachine * stateMachine) {
    Name = "ModelSelection";
    Widgets = new std::vector<tgui::Widget::Ptr>();
    Manager = stateMachine;
    WindowPack * Window = Manager->GetWindow();

    float WindowWidth = Window->GetSize().x;
    float WindowHeight = Window->GetSize().y;

    tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("/usr/share/tgui-0.7/widgets/Black.txt");

    tgui::Label::Ptr label = theme->load("label");
    label->setText("Wybor Modelu");
    label->setTextSize(30);
    label->setPosition(40, WindowHeight / 10.f);
    Window->GUIAdd(label);
    Widgets->push_back(label);

    label = theme->load("label");
    label->setText("Nazwa modelu");
    label->setPosition(40, (2 * WindowHeight) / 10.f + 5);
    Window->GUIAdd(label);
    Widgets->push_back(label);

    tgui::EditBox::Ptr editBox = theme->load("EditBox");
    editBox->setSize(200, 25);
    editBox->setTextSize(18);
    editBox->setPosition(40, (2 * WindowHeight) / 10.f + 35);
    Window->GUIAdd(editBox);
    Widgets->push_back(editBox);
    modelNamePtr = editBox;

    label = theme->load("label");
    label->setText("Folder w ktorym znajduje sie model");
    label->setPosition(40, (2 * WindowHeight) / 10.f + 65);
    Window->GUIAdd(label);
    Widgets->push_back(label);

    editBox = theme->load("EditBox");
    editBox->setSize(200, 25);
    editBox->setTextSize(18);
    editBox->setPosition(40, (2 * WindowHeight) / 10.f + 95);
    editBox->setText("../Models");
    Window->GUIAdd(editBox);
    Widgets->push_back(editBox);
    pathPtr = editBox;

    tgui::Button::Ptr button = theme->load("button");
    button = theme->load("button");
    button->setPosition(WindowWidth - 200, WindowHeight/10.f);
    button->setText("Wybierz");
    button->setSize(150, 40);
    button->connect("pressed", &ModelSelectionState::LoadModel, this);
    Window->GUIAdd(button);
    Widgets->push_back(button);

    button = theme->load("button");
    button->setPosition(WindowWidth - 115.f, WindowHeight - 50.f);
    button->setText("Wyjscie");
    button->setSize(100, 40);
    button->connect("pressed", [&] () { Manager->Close(); });
    Window->GUIAdd(button);
    Widgets->push_back(button);
}

void ModelSelectionState::LoadModel() {
    modelName = "";
    for (int i = 0; NULL != modelNamePtr->getText()[i]; i++)
        modelName += modelNamePtr->getText()[i];

    path = "";
    for (int i = 0; NULL != pathPtr->getText()[i]; i++)
        path += pathPtr->getText()[i];

    std::ifstream infile(path+"/"+modelName+".model");

    if (!infile.is_open()) {
        std::cout << "\nPrzykra sprawa\n";
    } else {
        std::getline(infile, modelName);
        string modelLvlStr;
        std::getline(infile, modelLvlStr);
        string nrOfRelsStr;
        std::getline(infile, nrOfRelsStr);
        //while getline push back to model relationships
    }

//    switch (ModelLevel) {
//        case 1:
//            NewModel = new Primitive();
//            break;
//        case 2:
//            NewModel = new Figure();
//            break;
//        case 3:
//            NewModel = new Solid();
//            break;
//        case 4:
//            NewModel = new Abstract();
//            break;
//        default:
//            break;
//    }
//
//    NewModel->SetName(ModelName);
//    NewModel->ChooseRelationships(ModelRelationships);

    infile.close();
}
