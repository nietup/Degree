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

	void GUIAdd(const tgui::Widget::Ptr & Wiget) { GUI->add(Wiget); };
	
	sf::Vector2u GetSize() { return Window->getSize(); };
private:
	tgui::Gui * GUI;
	sf::RenderWindow * Window;
};

