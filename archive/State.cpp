#include "State.h"

void State::HandleInput() {
}

void State::Update() {
}

void State::Render() {
}

void State::Show() {
	for (auto &Widget : *Widgets)
		Widget->show();
}

void State::Hide() {
	for (auto &Widget : *Widgets)
		Widget->hide();
}
