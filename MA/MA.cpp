#include <TGUI/TGUI.hpp>
#include "StateMachine.h"
#include "MenuState.h"

int main() {
	StateMachine SM(800, 600, 60, "Akwizycja Modeli");
	SM.ChangeState(new MenuState(&SM));

	while (true) {
		SM.Update();
		SM.Render();
	}

	return 0;
}

