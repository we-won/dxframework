#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

bool Camera::Render()
{
	XMVECTOR camPosition, camTarget, camUp;

	//Camera information
	_camPosition = XMFLOAT4(0.0f, 0.0f, -0.5f, 0.0f);
	_camTarget = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	_camUp = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	camPosition = XMLoadFloat4(&_camPosition);
	camTarget = XMLoadFloat4(&_camTarget);
	camUp = XMLoadFloat4(&_camUp);

	//Set the View matrix
	XMMATRIX view = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	XMStoreFloat4x4(&_view, view);

	return true;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return _view;
}