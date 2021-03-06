#include "Camera.h"


Camera::Camera()
{
	//Camera information
	m_camPosition = XMFLOAT3(0.0f, 20.0f, -8.0f);
	m_camTarget = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_camUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
}


Camera::~Camera()
{
}

void Camera::MoveLeft(float dt)
{
	m_camPosition.x -= 10.0f * dt;
	m_camTarget.x -= 10.0f * dt;

}

void Camera::MoveRight(float dt)
{
	m_camPosition.x += 10.0f * dt;
	m_camTarget.x += 10.0f * dt;
}

void Camera::MoveUp(float dt)
{
	m_camPosition.z += 10.0f * dt;
	m_camTarget.z += 10.0f * dt;
}

void Camera::MoveDown(float dt)
{
	m_camPosition.z -= 10.0f * dt;
	m_camTarget.z -= 10.0f * dt;
}

bool Camera::Render()
{
	XMVECTOR camPosition, camTarget, camUp;

	camPosition = XMLoadFloat3(&m_camPosition);
	camTarget = XMLoadFloat3(&m_camTarget);
	camUp = XMLoadFloat3(&m_camUp);

	//Set the View matrix
	XMMATRIX view = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	XMStoreFloat4x4(&m_view, view);

	return true;
}

