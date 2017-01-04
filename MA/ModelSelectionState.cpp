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
    //editBox->setDefaultText("Click to edit text...");
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
    //editBox->setDefaultText("Click to edit text...");
    Window->GUIAdd(editBox);
    Widgets->push_back(editBox);
    pathPtr = editBox;

//    label = theme->load("label");
//    label->setText("Wybierz typ modelu");
//    label->setPosition(40, (4*WindowHeight) / 10.f + 65);
//    Window->GUIAdd(label);
//    Widgets->push_back(label);
//
//    tgui::RadioButton::Ptr radioButton = theme->load("RadioButton");
//    radioButton->setPosition(40, 50 + ((5 * WindowHeight) / 10.f));
//    radioButton->setText("Prymityw");
//    radioButton->setSize(25, 25);
//    radioButton->connect("checked", [&] () {
//        ModelLevel = 1;
//        DisplayRelationships(1);
//    });
//    radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
//    Window->GUIAdd(radioButton);
//    Widgets->push_back(radioButton);
//
//    radioButton = theme->load("RadioButton");
//    radioButton->setText("Figura");
//    radioButton->setSize(25, 25);
//    radioButton->setPosition(40, 50 + ((5 * WindowHeight) / 10.f) + radioButton->getSize().y + 5);
//    radioButton->connect("checked", [&] () {
//        ModelLevel = 2;
//        DisplayRelationships(2);
//    });
//    radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
//    Window->GUIAdd(radioButton);
//    Widgets->push_back(radioButton);
//
//    radioButton = theme->load("RadioButton");
//    radioButton->setText("Bryla");
//    radioButton->setSize(25, 25);
//    radioButton->setPosition(40, 50 + ((5 * WindowHeight) / 10.f) + 2 * radioButton->getSize().y + 10);
//    radioButton->connect("checked", [&] () {
//        ModelLevel = 3;
//        DisplayRelationships(3);
//    });
//    radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
//    Window->GUIAdd(radioButton);
//    Widgets->push_back(radioButton);
//
//    radioButton = theme->load("RadioButton");
//    radioButton->setText("Abstrakt");
//    radioButton->setSize(25, 25);
//    radioButton->setPosition(40, 50 + ((5 * WindowHeight) / 10.f) + 3 * radioButton->getSize().y + 15);
//    radioButton->connect("checked", [&] () {
//        ModelLevel = 4;
//        DisplayRelationships(4);
//    });
//    radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
//    Window->GUIAdd(radioButton);
//    Widgets->push_back(radioButton);

    tgui::Button::Ptr button = theme->load("button");
    button = theme->load("button");
    button->setPosition(WindowWidth - 200, WindowHeight/10.f);
    button->setText("Wybierz");
    button->setSize(150, 40);
//    button->connect("pressed", &NewModelState::CreateModel, this);
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
