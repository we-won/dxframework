#include "ColorShader.h"


ColorShader::ColorShader()
{
	_VS = 0;
	_PS = 0;
	_vertLayout = 0;
	_cbPerObjectBuffer = 0;
}


ColorShader::~ColorShader()
{
}

bool ColorShader::Initialize(ID3D11Device* device)
{
	HRESULT hr;
	ID3D10Blob* _VS_Buffer;
	ID3D10Blob* _PS_Buffer;

	//Compile Shaders from shader file
	hr = D3DX11CompileFromFile("ColorEffects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &_VS_Buffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		return false;
	}

	hr = D3DX11CompileFromFile("ColorEffects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &_PS_Buffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading pixel shader!", "Compile Error", MB_OK);
		return false;
	}

	//Create the Shader Objects
	hr = device->CreateVertexShader(_VS_Buffer->GetBufferPointer(), _VS_Buffer->GetBufferSize(), NULL, &_VS);
	if (FAILED(hr))
	{
		if (_VS_Buffer)
			_VS_Buffer->Release();

		return false;
	}

	hr = device->CreatePixelShader(_PS_Buffer->GetBufferPointer(), _PS_Buffer->GetBufferSize(), NULL, &_PS);
	if (FAILED(hr))
	{
		if (_PS_Buffer)
			_PS_Buffer->Release();

		return false;
	}

	//Create input layout description
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	//Create the Input Layout
	hr = device->CreateInputLayout(layout, numElements, _VS_Buffer->GetBufferPointer(), _VS_Buffer->GetBufferSize(), &_vertLayout);
	if (FAILED(hr))
	{
		return false;
	}

	_VS_Buffer->Release();
	_VS_Buffer = 0;

	_PS_Buffer->Release();
	_PS_Buffer = 0;

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

	return true;
}

void ColorShader::ReleaseObjects()
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
}

bool ColorShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool ColorShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	//Set the World/View/Projection matrix, then send it to constant buffer in effect file
	XMMATRIX world, view, projection;

	world = XMLoadFloat4x4(&worldMatrix);
	view = XMLoadFloat4x4(&viewMatrix);
	projection = XMLoadFloat4x4(&projectionMatrix);

	XMMATRIX WVP = world * view * projection;
	XMStoreFloat4x4(&_WVP, XMMatrixTranspose(WVP));

	_cbPerObj.WVP = _WVP;

	deviceContext->UpdateSubresource(_cbPerObjectBuffer, 0, NULL, &_cbPerObj, 0, 0);

	deviceContext->VSSetConstantBuffers(0, 1, &_cbPerObjectBuffer);

	return true;
}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the Input Layout
	deviceContext->IASetInputLayout(_vertLayout);

	//Set Vertex and Pixel Shaders
	deviceContext->VSSetShader(_VS, 0, 0);
	deviceContext->PSSetShader(_PS, 0, 0);

	//Draw
	deviceContext->DrawIndexed(indexCount, 0, 0);
}