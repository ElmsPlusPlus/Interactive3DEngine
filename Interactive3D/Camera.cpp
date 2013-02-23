#include "Camera.h"


Camera::Camera(void)
{
	_defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	_defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	_defaultUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_cameraYaw = 0.0f;
	_cameraPitch = 0.0f;
	_cameraRoll = 0.0f;
	_moveLeftRight = 0.0f;
	_moveForwardBack = 0.0f;
	_cameraPosition = XMVectorSet(0.0f, 400.0f, 1.0f, 0.0f);
}


Camera::~Camera(void)
{
}

void Camera::Update(void)
{
	XMMATRIX cameraRotationYaw = XMMatrixRotationAxis(_defaultUp, _cameraYaw);
	_cameraRight = XMVector3TransformCoord(_defaultRight, cameraRotationYaw);
	_cameraForward = XMVector3TransformCoord(_defaultForward, cameraRotationYaw);

	XMMATRIX cameraRotationPitch = XMMatrixRotationAxis(_cameraRight, _cameraPitch);
	_cameraUp = XMVector3TransformCoord(_defaultUp, cameraRotationPitch);
	_cameraForward = XMVector3TransformCoord(_cameraForward, cameraRotationPitch);

	XMMATRIX cameraRotationRoll = XMMatrixRotationAxis(_cameraForward, _cameraRoll);
	_cameraUp = XMVector3TransformCoord(_cameraUp, cameraRotationRoll);
	_cameraRight = XMVector3TransformCoord(_cameraRight, cameraRotationRoll);

	_cameraPosition += _moveLeftRight * _cameraRight;
	_cameraPosition += _moveForwardBack * _cameraForward;

	_moveLeftRight = 0.0f;
	_moveForwardBack = 0.0f;

	XMVECTOR cameraTarget = _cameraPosition + XMVector3Normalize(_cameraForward);
	_viewMatrix = XMMatrixLookAtLH(_cameraPosition, cameraTarget, _cameraUp);
}

XMMATRIX Camera::GetViewMatrix(void)
{
	return _viewMatrix;
}

XMVECTOR Camera::GetCameraPosition(void)
{
	return _cameraPosition;
}

void Camera::SetCameraPosition(float x, float y, float z)
{
	_cameraPosition = XMVectorSet(x,y,z, 0);
}

void Camera::SetPitch(float pitch)
{
	_cameraPitch += pitch;
}

void Camera::SetYaw(float yaw)
{
	_cameraYaw += yaw;
}

void Camera::SetRoll(float roll)
{
	_cameraRoll += roll;
}

void Camera::SetLeftRight(float leftRight)
{
	_moveLeftRight = leftRight;
}

void Camera::SetForawrdBack(float forwardBack)
{
	_moveForwardBack = forwardBack;
}


