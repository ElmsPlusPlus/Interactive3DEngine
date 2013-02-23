#ifndef CAMERA_H
#define CAMERA_H
#include "core.h"
class Camera
{
public:
	Camera(void);
	~Camera(void);

	void Update(void);
	XMMATRIX GetViewMatrix(void);
	XMVECTOR GetCameraPosition(void);
	void SetCameraPosition(float x, float y, float z);
	void SetPitch(float pitch);
	void SetYaw(float yaw);
	void SetRoll(float roll);
	void SetLeftRight(float leftRight);
	void SetForawrdBack(float forwardBack);

private:
	XMVECTOR _defaultForward;
	XMVECTOR _defaultRight;
	XMVECTOR _defaultUp;
	XMVECTOR _cameraForward;
	XMVECTOR _cameraUp;
	XMVECTOR _cameraRight;
	XMVECTOR _cameraPosition;

	XMMATRIX _viewMatrix;
	
	float _moveLeftRight;
	float _moveForwardBack;

	float _cameraYaw;
	float _cameraPitch;
	float _cameraRoll;
};

#endif