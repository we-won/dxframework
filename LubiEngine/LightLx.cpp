#include "LightLx.h"


LightLx::LightLx()
{
}


LightLx::~LightLx()
{
}

bool LightLx::Initialize(ID3D11Device* device)
{
	HRESULT hr;

	hr = GenerateViewMatrix();
	if (FAILED(hr))
	{
		return false;
	}

	hr = GenerateProjectionMatrix();
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void LightLx::ReleaseObjects()
{
}

bool LightLx::GenerateViewMatrix()
{
	XMVECTOR camPosition, camTarget, camUp;

	XMFLOAT3 position = XMFLOAT3(4.0f, 10.0f, -4.0f);
	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	camPosition = XMLoadFloat3(&position);
	camTarget = XMLoadFloat3(&lookAt);
	camUp = XMLoadFloat3(&up);

	//Set the View matrix
	XMMATRIX view = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	XMStoreFloat4x4(&m_viewMatrix, view);

	return true;
}

bool LightLx::GenerateProjectionMatrix()
{
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1.0f, 0.1f, 1000.0f);
	XMStoreFloat4x4(&m_projectionMatrix, projection);

	return true;
}