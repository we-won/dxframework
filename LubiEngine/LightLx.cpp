#include "LightLx.h"


LightLx::LightLx()
: m_cbPerFrameBuffer(0)
{
	m_lookAt = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
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
	m_light.dir = XMFLOAT3(0.25f, 0.5f, -1.0f);
	m_light.ambient = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	m_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

void LightLx::SetLightEffectsOff()
{
	m_light.dir = XMFLOAT3(0.25f, 0.5f, -1.0f);
	m_light.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_light.diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

void LightLx::ApplyLight(ID3D11DeviceContext* deviceContext)
{	
	XMMATRIX viewMat, projectionMat;

	viewMat = XMLoadFloat4x4(&m_viewMatrix);
	projectionMat = XMLoadFloat4x4(&m_projectionMatrix);

	XMMATRIX VPMatrix = viewMat * projectionMat;
	
	XMFLOAT4X4 VP;
	XMStoreFloat4x4(&VP, XMMatrixTranspose(VPMatrix));
	
	m_constbuffPerFrame.light = m_light;
	m_constbuffPerFrame.VP = VP;

	deviceContext->UpdateSubresource(m_cbPerFrameBuffer, 0, NULL, &m_constbuffPerFrame, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_cbPerFrameBuffer);
}

bool LightLx::GenerateViewMatrix()
{
	XMVECTOR camPosition, camTarget, camUp;

	XMFLOAT4 position = XMFLOAT4(0.25f, 0.5f, -1.0f, 0.0f);
	m_lookAt = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	m_up = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	camPosition = XMLoadFloat4(&position);
	camTarget = XMLoadFloat4(&m_lookAt);
	camUp = XMLoadFloat4(&m_up);

	//Set the View matrix
	XMMATRIX view = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	XMStoreFloat4x4(&m_viewMatrix, view);

	return true;
}

bool LightLx::GenerateProjectionMatrix()
{
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, float(800)/float(600), 0.1f, 1000.0f);
	XMStoreFloat4x4(&m_projectionMatrix, projection);

	return true;
}