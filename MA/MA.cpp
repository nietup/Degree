#include <TGUI/TGUI.hpp>
#include "StateMachine.h"
#include "MenuState.h"

int main() {
	StateMachine SM(800, 600, 60, "Akwizycja Modeli");
	MenuState Menu(&SM);
	SM.ChangeState(&Menu);

	while (true) {
		SM.Update();
		SM.Render();
	}

	return 0;
}

