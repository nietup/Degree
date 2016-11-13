﻿#include "NewModelState.h"

NewModelState::NewModelState(StateMachine * SM) {
	Name = "NewModel";
	ModelName = "UDEF";
	ModelLevel = -1;
	ModelRelationships = new std::vector<int>();
	DisplayedRelationships = new std::vector<tgui::Widget::Ptr>();

	Widgets = new std::vector<tgui::Widget::Ptr>();
	Manager = SM;
	WindowPack * Window = Manager->GetWindow();

	float WindowWidth = Window->GetSize().x;
	float WindowHeight = Window->GetSize().y;

	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("../../widgets/Black.txt");

	tgui::Label::Ptr label = theme->load("label");
	label->setText("Nowy Model");
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
	ModelNamePtr = editBox;

	label = theme->load("label");
	label->setText("Wybierz typ modelu");
	label->setPosition(40, (4*WindowHeight) / 10.f + 5);
	Window->GUIAdd(label);
	Widgets->push_back(label);

	tgui::RadioButton::Ptr radioButton = theme->load("RadioButton");
	radioButton->setPosition(40, ((5 * WindowHeight) / 10.f))	;
	radioButton->setText("Prymityw");
	radioButton->setSize(25, 25);
	radioButton->connect("checked", [&] () {
		ModelLevel = 1;
		DisplayRelationships(1); 
	});
	radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
	Window->GUIAdd(radioButton);
	Widgets->push_back(radioButton);

	radioButton = theme->load("RadioButton");
	radioButton->setText("Bryla");
	radioButton->setSize(25, 25);
	radioButton->setPosition(40, ((5 * WindowHeight) / 10.f) + radioButton->getSize().y + 5);
	radioButton->connect("checked", [&] () {
		ModelLevel = 2;
		DisplayRelationships(2);
	});
	radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
	Window->GUIAdd(radioButton);
	Widgets->push_back(radioButton);

	radioButton = theme->load("RadioButton");
	radioButton->setText("Abstrakt");
	radioButton->setSize(25, 25);
	radioButton->setPosition(40, ((5 * WindowHeight) / 10.f) + 2 * radioButton->getSize().y + 10);
	radioButton->connect("checked", [&] () {
		ModelLevel = 3;
		DisplayRelationships(3);
	});
	radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
	Window->GUIAdd(radioButton);
	Widgets->push_back(radioButton);

	tgui::Button::Ptr button = theme->load("button");
	button = theme->load("button");
	button->setPosition(WindowWidth - 200, WindowHeight/10.f);
	button->setText("Stworz");
	button->setSize(150, 40);
	button->connect("pressed", &NewModelState::CreateModel, this);
	Window->GUIAdd(button);
	Widgets->push_back(button);

	button = theme->load("button");
	button->setPosition(15.f, WindowHeight - 50.f);
	button->setText("Menu glowne");
	button->setSize(300, 40); 
	button->connect("pressed", [&] () {
		if (!Manager->ChangeState("Menu"));;
			//Manager->ChangeState(new MenuState(Manager));
	});
	Window->GUIAdd(button);
	Widgets->push_back(button);

	button = theme->load("button");
	button->setPosition(WindowWidth - 115.f, WindowHeight - 50.f);
	button->setText("Exit");
	button->setSize(100, 40);
	button->connect("pressed", [&] () { Manager->Close(); });
	Window->GUIAdd(button);
	Widgets->push_back(button);
}

void NewModelState::DisplayRelationships(int Level) {

	DeselectAll();

	switch (Level) {
		case 1:
			NewModel = new Primitive();
			break;
		case 2:
			NewModel = new Solid();
			break;
		case 3:
			NewModel = new Abstract();
			break;
		default:
			break;
	}

	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("../../widgets/Black.txt");
	WindowPack * Window = Manager->GetWindow();
	tgui::CheckBox::Ptr checkbox;

	for (int i = 0; i < NewModel->NumberOfRelationships(); i++) {
		checkbox = theme->load("checkbox");
		checkbox->setPosition(420, 240 + i*25);
		checkbox->setText(NewModel->GetRelationshipName(i));
		checkbox->setSize(25, 25);
		checkbox->connect("checked", &NewModelState::Select, this, i);
		checkbox->connect("unchecked", &NewModelState::Deselect, this, i);
		Window->GUIAdd(checkbox);
		DisplayedRelationships->push_back(checkbox);
	}
}

void NewModelState::DeleteRelationships() {
	WindowPack * Window = Manager->GetWindow();

	for (auto &w : *DisplayedRelationships)
		Window->GUIDel(w);

	DisplayedRelationships->clear();
}

Model * NewModelState::CreateModel() {
	ModelName = "";
	for (int i = 0; NULL != ModelNamePtr->getText()[i]; i++)
		ModelName += ModelNamePtr->getText()[i];

	

	if ("" == ModelName || -1 == ModelLevel || 0 == ModelRelationships->size()) {
		WindowPack * Window = Manager->GetWindow();
		tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("../../widgets/Black.txt");
		tgui::ChildWindow::Ptr child = theme->load("ChildWindow");
		child->setSize(250, 120);
		child->setPosition(420, 80);
		child->setTitle("Child window");
		Window->GUIAdd(child);

		tgui::Label::Ptr label = theme->load("label");
		label->setText("Prosze podac wszystkie dane");
		label->setPosition(30, 30);
		label->setTextSize(15);
		child->add(label);

		tgui::Button::Ptr button = theme->load("button");
		button->setPosition(75, 70);
		button->setText("OK");
		button->setSize(100, 30);
		button->connect("pressed", [=] () { child->hide(); });
		child->add(button);
		return nullptr;
	}

	std::cout << "\nModel Name: " << ModelName << "\nModel Level: " << ModelLevel << "\nModel Relationships: ";
	for (int &i : *ModelRelationships)
		std::cout << i << " ";

	NewModel->SetName(ModelName);
	NewModel->ChooseRelationships(ModelRelationships);

	for (int i = 0; i < NewModel->NumberOfRelationships(); i++)
		std::cout << "\n" << NewModel->GetRelationshipName(i);

	return NewModel;
}

void NewModelState::Show() {
	for (auto &Widget : *Widgets)
		Widget->show();

	for (auto &Widget : *DisplayedRelationships)
		Widget->show();
}

void NewModelState::Hide() {
	for (auto &Widget : *Widgets)
		Widget->hide();

	for (auto &Widget : *DisplayedRelationships)
		Widget->hide();
}

void NewModelState::Select(int i) {
	std::vector<int>::iterator it;
	it = find(ModelRelationships->begin(), ModelRelationships->end(), i);

	if (it == ModelRelationships->end())
		ModelRelationships->push_back(i);
}

void NewModelState::Deselect(int i) {
	std::vector<int>::iterator it;
	it = find(ModelRelationships->begin(), ModelRelationships->end(), i);

	if (it != ModelRelationships->end())
		ModelRelationships->erase(it);
}

void NewModelState::DeselectAll() {
	ModelRelationships->clear();
}