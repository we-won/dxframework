#include "LightLx.h"


LightLx::LightLx()
: m_cbPerFrameBuffer(0)
{
}


LightLx::~LightLx()
{
}

bool LightLx::Initialize(ID3D11Device* device)
{
	HRESULT hr;

	//Create the buffer to send to the cbuffer per frame in effect file
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = device->CreateBuffer(&cbbd, NULL, &m_cbPerFrameBuffer);
	if (FAILED(hr))
	{
		return false;
	}

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
	if (m_cbPerFrameBuffer)
	{
		m_cbPerFrameBuffer->Release();
		m_cbPerFrameBuffer = 0;
	}
}

void LightLx::SetLightEffectsOn()
{
	m_light.dir = XMFLOAT3(1.0f, 8.0f, -5.0f);
	m_light.ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	m_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void LightLx::SetLightEffectsOff()
{
	m_light.dir = XMFLOAT3(1.0f, 8.0f, -5.0f);
	m_light.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_light.diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void LightLx::ApplyLight(ID3D11DeviceContext* deviceContext)
{	
	//m_constbuffPerFrame.light = m_light;

	//deviceContext->UpdateSubresource(m_cbPerFrameBuffer, 0, NULL, &m_constbuffPerFrame, 0, 0);
	//deviceContext->PSSetConstantBuffers(0, 1, &m_cbPerFrameBuffer);
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