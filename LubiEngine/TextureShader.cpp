#include "TextureShader.h"


TextureShader::TextureShader()
: m_VS(0), m_PS(0), m_vertLayout(0), m_cbPerObjectBuffer(0), m_texSamplerState(0)
{
}


TextureShader::~TextureShader()
{
}

bool TextureShader::Initialize(ID3D11Device* device)
{
	HRESULT hr;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	//Compile Shaders from shader file
	hr = D3DX11CompileFromFile("Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		return false;
	}

	hr = D3DX11CompileFromFile("Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = device->CreateSamplerState(&sampDesc, &m_texSamplerState);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void TextureShader::ReleaseObjects()
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

bool TextureShader::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, int indexCount, ID3D11ShaderResourceView* texture,
	XMFLOAT4X4 lightViewMatrix, XMFLOAT4X4 lightProjectionMatrix, ID3D11ShaderResourceView* depthMapTexture)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightViewMatrix, lightProjectionMatrix, depthMapTexture))
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture,
	XMFLOAT4X4 lightViewMatrix, XMFLOAT4X4 lightProjectionMatrix, ID3D11ShaderResourceView* depthMapTexture)
{
	//Set the World/View/Projection matrix, then send it to constant buffer in effect file
	XMMATRIX worldMat, viewMat, projectionMat;

	worldMat = XMLoadFloat4x4(&worldMatrix);
	viewMat = XMLoadFloat4x4(&viewMatrix);
	projectionMat = XMLoadFloat4x4(&projectionMatrix);

	XMMATRIX WVPMatrix = worldMat * viewMat * projectionMat;
	
	XMFLOAT4X4 WVP;
	XMStoreFloat4x4(&WVP, XMMatrixTranspose(WVPMatrix));

	m_cbPerObj.WVP = WVP;
	
	XMFLOAT4X4 World;
	XMStoreFloat4x4(&World, XMMatrixTranspose(worldMat));

	m_cbPerObj.World = World;

	XMMATRIX lightViewMat, lightProjectionMat;
	
	XMMATRIX LightWVPMatrix = worldMat * lightViewMat * lightProjectionMat;
	
	XMFLOAT4X4 LightWVP;
	XMStoreFloat4x4(&LightWVP, XMMatrixTranspose(LightWVPMatrix));

	m_cbPerObj.lightWVP = LightWVP;

	deviceContext->UpdateSubresource(m_cbPerObjectBuffer, 0, NULL, &m_cbPerObj, 0, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &depthMapTexture);

	deviceContext->PSSetSamplers(0, 1, &m_texSamplerState);

	return true;
}

void TextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the Input Layout
	deviceContext->IASetInputLayout(m_vertLayout);

	//Set Vertex and Pixel Shaders
	deviceContext->VSSetShader(m_VS, 0, 0);
	deviceContext->PSSetShader(m_PS, 0, 0);

	//Draw
	deviceContext->DrawIndexed(indexCount, 0, 0);
}