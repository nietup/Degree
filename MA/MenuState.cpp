#include "MenuState.h"


MenuState::MenuState(StateMachine * SM) {
	Name = "Menu";
	Widgets = new std::vector <tgui::Widget::Ptr>();
	Manager = SM;
	WindowPack * Window = Manager->GetWindow();

	float WindowWidth = Window->GetSize().x;
	float WindowHeight = Window->GetSize().y;

	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("../../widgets/Black.txt");

	tgui::Label::Ptr label = theme->load("label");
	label->setText("Akwizycja Modeli");
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	label->setPosition(WindowWidth/2.f, WindowHeight/10.f);
	label->setTextSize(30);
	Window->GUIAdd(label);
	Widgets->push_back(label);

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(WindowWidth/2.f - 50.f, WindowHeight/2.f + 10);
	button->setText("Nowy model");
	button->setSize(300, 40);
	button->connect("pressed", [&] () {
		if (!Manager->ChangeState("NewModel"))
			Manager->ChangeState(new NewModelState(Manager)); 
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

void MenuState::HandleInput() {
	/*if (input == KEY_ESC) {
		exit();
	}
	else if (input == KEY_1) {
		// showLevelsToChoose();
		stateMachine->changeState(new LevelChoice);
	}
	else if (input == KEY_2) {
		// startDisplayingCredits();
		stateMachine->changeState(new Credits);
	}*/
}

