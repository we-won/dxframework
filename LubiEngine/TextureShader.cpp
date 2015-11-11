#include "TextureShader.h"


TextureShader::TextureShader()
: m_VS(0), m_PS(0), m_vertLayout(0),
	m_texSamplerState(0),
	m_cbPerObjectBuffer(0), m_lightBuffer(0), m_lightBuffer2(0)
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MipLODBias = 0.0f;
    sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = 0;
	sampDesc.BorderColor[1] = 0;
	sampDesc.BorderColor[2] = 0;
	sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = device->CreateSamplerState(&sampDesc, &m_texSamplerState);
	if (FAILED(hr))
	{
		return false;
	}

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

	//Create the buffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC lightBufferDesc;
	ZeroMemory(&lightBufferDesc, sizeof(D3D11_BUFFER_DESC));

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = 0;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	hr = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(hr))
	{
		return false;
	}

	//Create the buffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC lightBufferDesc2;
	ZeroMemory(&lightBufferDesc2, sizeof(D3D11_BUFFER_DESC));

	// Setup the description of the light dynamic constant buffer that is in the vertex shader.
	lightBufferDesc2.Usage = D3D11_USAGE_DEFAULT;
	lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = 0;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	hr = device->CreateBuffer(&lightBufferDesc2, NULL, &m_lightBuffer2);
	if(FAILED(hr))
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

	if (m_texSamplerState)
	{
		m_texSamplerState->Release();
		m_texSamplerState = 0;
	}

	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	if (m_lightBuffer2)
	{
		m_lightBuffer2->Release();
		m_lightBuffer2 = 0;
	}
}

bool TextureShader::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, 
							XMFLOAT4X4 projectionMatrix, int indexCount, ID3D11ShaderResourceView* texture, 
							XMFLOAT4X4 lightViewMatrix, XMFLOAT4X4 lightProjectionMatrix, 
							ID3D11ShaderResourceView* depthMapTexture)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightViewMatrix, lightProjectionMatrix, depthMapTexture))
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, 
										XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4X4 lightViewMatrix, 
										XMFLOAT4X4 lightProjectionMatrix, ID3D11ShaderResourceView* depthMapTexture)
{
	//Set the World/View/Projection matrix, then send it to constant buffer in effect file
	XMMATRIX worldMat, viewMat, projectionMat;
	XMMATRIX lightViewMat, lightProjectionMat;
	
	cbPerObject cbPerObj;
	LightBufferType light1;
	LightBufferType2 light2;

	worldMat = XMLoadFloat4x4(&worldMatrix);
	viewMat = XMLoadFloat4x4(&viewMatrix);
	projectionMat = XMLoadFloat4x4(&projectionMatrix);
	lightViewMat = XMLoadFloat4x4(&lightViewMatrix);
	lightProjectionMat = XMLoadFloat4x4(&lightProjectionMatrix);

	XMMATRIX WVPMatrix = worldMat * viewMat * projectionMat;
	XMMATRIX LightWVPMatrix = worldMat * lightViewMat * lightProjectionMat;

	XMFLOAT4X4 WVP;
	XMFLOAT4X4 World;
	XMFLOAT4X4 LightWVP;

	XMStoreFloat4x4(&WVP, XMMatrixTranspose(WVPMatrix));
	XMStoreFloat4x4(&World, XMMatrixTranspose(worldMat));
	XMStoreFloat4x4(&LightWVP, XMMatrixTranspose(LightWVPMatrix));

	// Ambient and Diffuse

	light1.ambientColor = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	light1.diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	deviceContext->UpdateSubresource(m_lightBuffer, 0, NULL, &light1, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &m_lightBuffer);

	// Per object

	cbPerObj.WVP = WVP;
	cbPerObj.World = World;
	cbPerObj.LightWVP = LightWVP;

	deviceContext->UpdateSubresource(m_cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);

	// Light position

	light2.lightPosition = XMFLOAT3(5.0f, 5.0f, -5.0f);
	light2.padding = 0.0f;

	deviceContext->UpdateSubresource(m_lightBuffer2, 0, NULL, &light2, 0, 0);
	deviceContext->VSSetConstantBuffers(1, 1, &m_lightBuffer2);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &depthMapTexture);

	return true;
}

void TextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the Input Layout
	deviceContext->IASetInputLayout(m_vertLayout);

	//Set Vertex and Pixel Shaders
	deviceContext->VSSetShader(m_VS, 0, 0);
	deviceContext->PSSetShader(m_PS, 0, 0);
	
	// Set the sampler states in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_texSamplerState);

	//Draw
	deviceContext->DrawIndexed(indexCount, 0, 0);
}