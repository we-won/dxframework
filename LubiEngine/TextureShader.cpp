#include "TextureShader.h"


TextureShader::TextureShader()
{
	_VS = 0;
	_PS = 0;
	_vertLayout = 0;
	_cbPerObjectBuffer = 0;
	_texSamplerState = 0;
	_cbPerFrameBuffer = 0;
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
	hr = device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &_VS);
	if (FAILED(hr))
	{
		if (VS_Buffer)
			VS_Buffer->Release();

		return false;
	}

	hr = device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &_PS);
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
	hr = device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &_vertLayout);
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

	hr = device->CreateBuffer(&cbbd, NULL, &_cbPerObjectBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	//Create the buffer to send to the cbuffer per frame in effect file
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = device->CreateBuffer(&cbbd, NULL, &_cbPerFrameBuffer);

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
	hr = device->CreateSamplerState(&sampDesc, &_texSamplerState);

	_light.dir = XMFLOAT3(0.25f, 0.5f, -1.0f);
	_light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	_light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	_light.dir = XMFLOAT3(0.0f, 0.0f, 0.0f);
	_light.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	_light.diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	return true;
}

void TextureShader::ReleaseObjects()
{
	if (_VS)
	{
		_VS->Release();
		_VS = 0;
	}

	if (_PS)
	{
		_PS->Release();
		_PS = 0;
	}

	if (_vertLayout)
	{
		_vertLayout->Release();
		_vertLayout = 0;
	}

	if (_cbPerObjectBuffer)
	{
		_cbPerObjectBuffer->Release();
		_cbPerObjectBuffer = 0;
	}

	if (_texSamplerState)
	{
		_texSamplerState->Release();
		_texSamplerState = 0;
	}

	if (_cbPerFrameBuffer)
	{
		_cbPerFrameBuffer->Release();
		_cbPerFrameBuffer = 0;
	}
}

bool TextureShader::Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, int indexCount, ID3D11ShaderResourceView* texture)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture))
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture)
{
	//Set the World/View/Projection matrix, then send it to constant buffer in effect file
	XMMATRIX worldMat, viewMat, projectionMat;

	worldMat = XMLoadFloat4x4(&worldMatrix);
	viewMat = XMLoadFloat4x4(&viewMatrix);
	projectionMat = XMLoadFloat4x4(&projectionMatrix);

	_constbuffPerFrame.light = _light;
	deviceContext->UpdateSubresource(_cbPerFrameBuffer, 0, NULL, &_constbuffPerFrame, 0, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &_cbPerFrameBuffer);

	XMMATRIX WVPMatrix = worldMat * viewMat * projectionMat;
	
	XMFLOAT4X4 WVP;
	XMStoreFloat4x4(&WVP, XMMatrixTranspose(WVPMatrix));

	_cbPerObj.WVP = WVP;
	
	XMFLOAT4X4 World;
	XMStoreFloat4x4(&World, XMMatrixTranspose(worldMat));

	_cbPerObj.World = World;

	deviceContext->UpdateSubresource(_cbPerObjectBuffer, 0, NULL, &_cbPerObj, 0, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &_cbPerObjectBuffer);

	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetSamplers(0, 1, &_texSamplerState);

	return true;
}

void TextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the Input Layout
	deviceContext->IASetInputLayout(_vertLayout);

	//Set Vertex and Pixel Shaders
	deviceContext->VSSetShader(_VS, 0, 0);
	deviceContext->PSSetShader(_PS, 0, 0);

	//Draw
	deviceContext->DrawIndexed(indexCount, 0, 0);
}