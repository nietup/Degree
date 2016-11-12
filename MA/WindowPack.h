#pragma once
#include <TGUI/TGUI.hpp>
#include <string.h>

/*
The purpose of this class is to hold Window, GUI object and all information about them
*/

using namespace std;

class WindowPack {
public:
	WindowPack(int, int, int, string);
	~WindowPack();

	void Render();
	void HandleEvent();
	void Close(); //read access violation

	void GUIAdd(const tgui::Widget::Ptr & Widget) { GUI->add(Widget); };
	void GUIDel(const tgui::Widget::Ptr & Widget) { GUI->remove(Widget); };
	
	sf::Vector2u GetSize() { return Window->getSize(); };

private:
	tgui::Gui * GUI;
	sf::RenderWindow * Window;
};

