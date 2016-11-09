#include "MenuState.h"


MenuState::MenuState(WindowPack*) {
	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("../../widgets/Black.txt");

	tgui::Label::Ptr label = theme->load("label");
	label->setText("Akwizycja Modeli");
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	label->setPosition(window.getSize().x / 2.f, window.getSize().y / 10.f);
	label->setTextSize(30);
	GUI->add(label);

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(window.getSize().x / 2.f - 50.f, window.getSize().y / 2.f + 10);
	button->setText("Wybór modelu");
	button->setSize(300, 40);
	GUI->add(button);

	button = theme->load("button");
	button->setPosition(window.getSize().x - 115.f, window.getSize().y - 50.f);
	button->setText("Exit");
	button->setSize(100, 40);
	button->connect("pressed", [&] () { window.close(); });
	GUI->add(button);
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