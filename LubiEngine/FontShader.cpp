#include "FontShader.h"


FontShader::FontShader() 
: m_VS(0), m_PS(0), m_vertLayout(0), m_cbPerObjectBuffer(0), m_texSamplerState(0)
{
}


FontShader::~FontShader()
{
}

bool FontShader::Initialize(ID3D11Device* device)
{
	HRESULT hr;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	//Compile Shaders from shader file
	hr = D3DX11CompileFromFile("FontEffects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		return false;
	}

	hr = D3DX11CompileFromFile("FontEffects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading pixel shader!", "Compile Error", MB_OK);
		return false;
	}

	//Create the Shader Objects
	hr = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &m_VS);
	if (FAILED(hr))
	{
		if (VS_Buffer)
			VS_Buffer->Release();

		return false;
	}

	hr = device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &m_PS);
	if (FAILED(hr))
	{
		if (PS_Buffer)
			PS_Buffer->Release();

		return false;
	}

	//Create input layout description
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	//Create the Input Layout
	hr = device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &m_vertLayout);
	if (FAILED(hr))
	{
		return false;
	}

	VS_Buffer->Release();
	VS_Buffer = 0;

	PS_Buffer->Release();
	PS_Buffer = 0;

	//Create the buffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = device->CreateBuffer(&cbbd, NULL, &m_cbPerObjectBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = device->CreateSamplerState(&sampDesc, &m_texSamplerState);

	return true;
}

void FontShader::ReleaseObjects()
{
	if (m_VS)
	{
		m_VS->Release();
		m_VS = 0;
	}

	if (m_PS)
	{
		m_PS->Release();
		m_PS = 0;
	}

	if (m_vertLayout)
	{
		m_vertLayout->Release();
		m_vertLayout = 0;
	}

	if (m_cbPerObjectBuffer)
	{
		m_cbPerObjectBuffer->Release();
		m_cbPerObjectBuffer = 0;
	}

	if (m_texSamplerState)
	{
		m_texSamplerState->Release();
		m_texSamplerState = 0;
	}
}

bool FontShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 orthoMatrix, ID3D11ShaderResourceView* texture)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, orthoMatrix, texture))
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool FontShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 orthoMatrix, ID3D11ShaderResourceView* texture)
{
	//Set the World/View/Projection matrix, then send it to constant buffer in effect file
	//XMMATRIX worldMat, viewMat, projectionMat;

	//worldMat = XMLoadFloat4x4(&worldMatrix);
	////viewMat = XMLoadFloat4x4(&viewMatrix);
	//viewMat = XMMatrixIdentity();
	////projectionMat = XMLoadFloat4x4(&orthoMatrix);
	//projectionMat = XMMatrixIdentity();

	//XMMATRIX WVP = worldMat * viewMat * projectionMat;

	XMMATRIX WVPMatrix = XMMatrixIdentity();

	XMFLOAT4X4 WVP;
	XMStoreFloat4x4(&WVP, XMMatrixTranspose(WVPMatrix));

	m_cbPerObj.WVP = WVP;

	deviceContext->UpdateSubresource(m_cbPerObjectBuffer, 0, NULL, &m_cbPerObj, 0, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &m_texSamplerState);

	return true;
}

void FontShader::RenderShader(ID3D11DeviceContext* deviceContext, int vertexCount)
{
	//Set the Input Layout
	deviceContext->IASetInputLayout(m_vertLayout);

	//Set Vertex and Pixel Shaders
	deviceContext->VSSetShader(m_VS, 0, 0);
	deviceContext->PSSetShader(m_PS, 0, 0);

	//Draw
	deviceContext->Draw(vertexCount, 0);
}