#include "MenuState.h"


MenuState::MenuState(StateMachine * SM) {
	WindowPack * Window = SM->GetWindow();

	float WindowWidth = Window->GetSize().x;
	float WindowHeight = Window->GetSize().y;

	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("../../widgets/Black.txt");

	tgui::Label::Ptr label = theme->load("label");
	label->setText("Akwizycja Modeli");
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	label->setPosition(WindowWidth/2.f, WindowHeight/10.f);
	label->setTextSize(30);
	Window->GUIAdd(label);

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(WindowWidth/2.f - 50.f, WindowHeight/2.f + 10);
	button->setText("Wybór modelu");
	button->setSize(300, 40);
	Window->GUIAdd(button);

	button = theme->load("button");
	button->setPosition(WindowWidth - 115.f, WindowHeight - 50.f);
	button->setText("Exit");
	button->setSize(100, 40);
	button->connect("pressed", [&] () { Window->Close(); });
	Window->GUIAdd(button);
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

void MenuState::Update() {
}

void MenuState::Render() {
}

void MenuState::Show() {
}

void MenuState::Hide() {
}
