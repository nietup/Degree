#include "SampleSelectionState.h"



SampleSelectionState::SampleSelectionState(StateMachine* SM, Model * M) {

	Name = "SampleSelection";
	Widgets = new std::vector<tgui::Widget::Ptr>();
	Manager = SM;
	WindowPack * Window = Manager->GetWindow();
	LearnedModel = M;

	float WindowWidth = Window->GetSize().x;
	float WindowHeight = Window->GetSize().y;

	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("../../widgets/Black.txt");

	tgui::Label::Ptr label = theme->load("label");
	label->setText("Wybor Probek");
	label->setTextSize(30);
	label->setPosition(40, WindowHeight / 10.f);
	Window->GUIAdd(label);
	Widgets->push_back(label);

	label = theme->load("label");
	label->setText("Podaj sciezke do folderu z przykladami pozytywnymi");
	label->setPosition(40, (2 * WindowHeight) / 10.f + 25);
	Window->GUIAdd(label);
	Widgets->push_back(label);

	tgui::EditBox::Ptr editBox = theme->load("EditBox");
	editBox->setSize(200, 25);
	editBox->setTextSize(18);
	editBox->setPosition(40, (2 * WindowHeight) / 10.f + 55);
	editBox->setDefaultText(".\\samples\\pos\\");
	Window->GUIAdd(editBox);
	Widgets->push_back(editBox);
	PositiveFolder = editBox;

	label = theme->load("label");
	label->setText("Podaj sciezke do folderu z przykladami negatywnymi");
	label->setPosition(40, (4 * WindowHeight) / 10.f + 25);
	Window->GUIAdd(label);
	Widgets->push_back(label);

	editBox = theme->load("EditBox");
	editBox->setSize(200, 25);
	editBox->setTextSize(18);
	editBox->setPosition(40, (4 * WindowHeight) / 10.f + 55);
	editBox->setDefaultText(".\\samples\\neg\\");
	Window->GUIAdd(editBox);
	Widgets->push_back(editBox);
	PositiveFolder = editBox;

	tgui::Button::Ptr button = theme->load("button");
	button = theme->load("button");
	button->setPosition(WindowWidth - 200, WindowHeight / 10.f);
	button->setText("Ucz");
	button->setSize(150, 40);
	//button->connect("pressed", &NewModelState::CreateModel, this);
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


SampleSelectionState::~SampleSelectionState() {
}
