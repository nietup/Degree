
#include <TGUI/TGUI.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Akwizycja Modeli");
	window.setFramerateLimit(60);

	tgui::Gui gui(window);

	tgui::Theme::Ptr theme = std::make_shared<tgui::Theme>("../../widgets/Black.txt");

	tgui::Label::Ptr label = theme->load("label");
	label->setText("Akwizycja Modeli");
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	label->setPosition(window.getSize().x/2.f - label->getFullSize().x/2, window.getSize().y/10.f);
	label->setTextSize(30);
	gui.add(label);

	tgui::Button::Ptr button = theme->load("button");
	button->setPosition(window.getSize().x/2.f - 50.f, window.getSize().y/2.f + 10);
	button->setText("Wybór modelu");
	button->setSize(300, 40);
	gui.add(button);

	/*button = theme->load("button");
	button->setPosition(window.getSize().x / 2.f - 50.f, window.getSize().y / 2.f - 10);
	button->setText("Nowy model");
	button->setSize(300, 40);
	gui.add(button);
	*/
	button = theme->load("button");
	button->setPosition(window.getSize().x - 115.f, window.getSize().y - 50.f);
	button->setText("Exit");
	button->setSize(100, 40);
	button->connect("pressed", [&] () { window.close(); });
	gui.add(button);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			gui.handleEvent(event);
		}

		window.clear();
		gui.draw();
		window.display();
	}

	return 0;
}

