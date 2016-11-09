#include "WindowPack.h"



WindowPack::WindowPack(int WindowWidth, int WindowHeight, int Rate, string Title) {
	Window = new sf::RenderWindow(sf::VideoMode(WindowWidth, WindowHeight), Title);
	Window->setFramerateLimit(Rate);

	GUI = new tgui::Gui(*Window);
}


WindowPack::~WindowPack() {
	delete Window;
	delete GUI;
}
