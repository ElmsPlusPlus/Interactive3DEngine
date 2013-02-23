#pragma once
#include "framework.h"
#include "BiPlaneNode.h"
#include "TerrainNode.h"
#include "WoodenCube.h"
#include "SkyBox.h"

// Main Class of the of the program. Overrides the Framework to make the engine do more than just a blank screen

// Inheirits from Framework
class Interactive3D : public Framework
{
    // Overridden methods from Framework
    wstring WindowCaption(void);
    RECT WindowDimensions(void);
	void Initialise(void);
	bool HandleKeyboardMessage(WPARAM wParam, LPARAM lParam);
};




