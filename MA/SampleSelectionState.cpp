#include "SampleSelectionState.h"
#include <dirent.h>
#include <iostream>

SampleSelectionState::SampleSelectionState(StateMachine* SM, Model * M) {

	Name = "SampleSelection";
	Widgets = new std::vector<tgui::Widget::Ptr>();
	Manager = SM;
	WindowPack * Window = Manager->GetWindow();
	LearnedModel = M;

	float WindowWidth = Window->GetSize().x;
	float WindowHeight = Window->GetSize().y;

	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("/usr/share/tgui-0.7/widgets/Black.txt");

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
	editBox->setText("..\\Samples\\pos\\");
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
	editBox->setText("..\\Samples\\neg\\");
	Window->GUIAdd(editBox);
	Widgets->push_back(editBox);
	PositiveFolder = editBox;

	tgui::Button::Ptr button = theme->load("button");
	button = theme->load("button");
	button->setPosition(WindowWidth - 200, WindowHeight / 10.f);
	button->setText("Ucz");
	button->setSize(150, 40);
	button->connect("pressed", &SampleSelectionState::PrepareLearningData, this);
	Window->GUIAdd(button);
	Widgets->push_back(button);

	button = theme->load("button");
	button->setPosition(15.f, WindowHeight - 50.f);
	button->setText("Powrot");
	button->setSize(100, 40);
	button->connect("pressed", [&] () {
		if (!Manager->ChangeState("NewModel"));
		//Manager->ChangeState(new MenuState(Manager));
	});
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


SampleSelectionState::~SampleSelectionState() {
}

void SampleSelectionState::SetPathStrings() {
    //TODO
    PositivePath = "/home/nietup/Code/eng/Degree/MA/Samples/pos";
    NegativePath = "/home/nietup/Code/eng/Degree/MA/Samples/neg";
}

void SampleSelectionState::PrepareLearningData() {
    SetPathStrings();

	DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (PositivePath.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            //std::cout << "\n" << ent->d_name << " - |" << (int)ent->d_type << "|"; //8 is file 4 is folder
            if (8 == ent->d_type)
                PositiveFiles->push_back(string(ent->d_name));
        }
        closedir (dir);
    }
    else {
        /* could not open directory */
        perror ("");
        std::cout << "\nPROGRAM FAILURE: Unable to access path " << PositivePath << "\n";
    }

    for (auto &i : *PositiveFiles)
        std::cout << "\n" << i;
    Handler = new ImageHandler(PositiveFiles);
    Handler->ImageToLearningSample();
}
