#ifndef GAMEPADCONTROLLER_H
#define GAMEPADCONTROLLER_H

#include "core.h"
#include <XInput.h>
#include "Camera.h"
#pragma comment(lib, "XInput.lib")

class Framework;

class GamePadController
{
public:
	GamePadController(void);
	~GamePadController(void);
	void ProcessGameController(Camera* camera);

private:
	XINPUT_STATE _controllerState;
	DWORD		 _lastPacketNumber;
	bool		 _firstTime;

	// These two values are used to avoid having to calculate square roots (which are very time consuming)
	// when we are checking if the movement of the left or right thumb stick is in the dead zone
	DWORD		_leftThumbDeadZoneSquared;
	DWORD		_rightThumbDeadZoneSquared;
};

#endif