#include "NewModelState.h"

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

	label = theme->load("label");
	label->setText("Wybierz typ modelu");
	label->setPosition(40, (4*WindowHeight) / 10.f + 5);
	Window->GUIAdd(label);
	Widgets->push_back(label);

	tgui::RadioButton::Ptr radioButton = theme->load("RadioButton");
	radioButton->setPosition(40, ((5 * WindowHeight) / 10.f))	;
	radioButton->setText("Prymityw");
	radioButton->setSize(25, 25);
	radioButton->connect("checked", [&] () {DisplayRelationships(1); });
	radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
	Window->GUIAdd(radioButton);
	Widgets->push_back(radioButton);

	radioButton = theme->load("RadioButton");
	radioButton->setText("Bryla");
	radioButton->setSize(25, 25);
	radioButton->setPosition(40, ((5 * WindowHeight) / 10.f) + radioButton->getSize().y + 5);
	radioButton->connect("checked", [&] () {DisplayRelationships(2); });
	radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
	Window->GUIAdd(radioButton);
	Widgets->push_back(radioButton);

	radioButton = theme->load("RadioButton");
	radioButton->setText("Abstrakt");
	radioButton->setSize(25, 25);
	radioButton->setPosition(40, ((5 * WindowHeight) / 10.f) + 2 * radioButton->getSize().y + 10);
	radioButton->connect("checked", [&] () {DisplayRelationships(3); });
	radioButton->connect("unchecked", [&] () {DeleteRelationships(); });
	Window->GUIAdd(radioButton);
	Widgets->push_back(radioButton);

	//TODO Button "stworz model"

	tgui::Button::Ptr button = theme->load("button");
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

	Model * NewModel = new Model();

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
		checkbox->connect("checked", &NewModelState::printuj, this, i);
//		checkbox->connect("unchecked", [&] () {DeleteRelationships(); });
		Window->GUIAdd(checkbox);
		DisplayedRelationships->push_back(checkbox);
	}

	delete NewModel;
}

void NewModelState::DeleteRelationships() {
	WindowPack * Window = Manager->GetWindow();

	for (auto &w : *DisplayedRelationships)
		Window->GUIDel(w);

	DisplayedRelationships->clear();
}

void NewModelState::Show() {
	for (auto &Widget : *Widgets)
		Widget->show();

	for (auto &Widget : *DisplayedRelationships)
		Widget->show();
}
