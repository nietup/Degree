
#include <TGUI/TGUI.hpp>

int main() {
	

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

