#pragma once
#include <TGUI/TGUI.hpp>
#include "WindowPack.h"

class State {
public:
	virtual void HandleInput() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;
};
