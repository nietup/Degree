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

void WindowPack::Render() {
	Window->clear();
	GUI->draw();
	Window->display();
}

void WindowPack::HandleEvent() {
	sf::Event event;
	while (Window->pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			Window->close();

		GUI->handleEvent(event);
	}
}
