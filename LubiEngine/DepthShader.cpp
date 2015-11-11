#include "DepthShader.h"


DepthShader::DepthShader()
: m_VS(0), m_PS(0), m_vertLayout(0), m_cbPerObjectBuffer(0)
{
}


DepthShader::~DepthShader()
{
}

bool DepthShader::Initialize(ID3D11Device* device)
{
	HRESULT hr;
	ID3D10Blob* VS_Buffer;
	ID3D10Blob* PS_Buffer;

	//Compile Shaders from shader file
	hr = D3DX11CompileFromFile("DepthEffects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading depth vertex shader!", "Compile Error", MB_OK);
		return false;
	}

	hr = D3DX11CompileFromFile("DepthEffects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading depth pixel shader!", "Compile Error", MB_OK);
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	cbbd.StructureByteStride = 0;

	hr = device->CreateBuffer(&cbbd, NULL, &m_cbPerObjectBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void DepthShader::ReleaseObjects()
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
}

bool DepthShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}

bool DepthShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	//Set the World/View/Projection matrix, then send it to constant buffer in effect file
	XMMATRIX world, view, projection;

	world = XMLoadFloat4x4(&worldMatrix);
	view = XMLoadFloat4x4(&viewMatrix);
	projection = XMLoadFloat4x4(&projectionMatrix);

	XMMATRIX WVP = world * view * projection;
	XMStoreFloat4x4(&m_WVP, XMMatrixTranspose(WVP));

	m_cbPerObj.WVP = m_WVP;

	deviceContext->UpdateSubresource(m_cbPerObjectBuffer, 0, NULL, &m_cbPerObj, 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_cbPerObjectBuffer);

	return true;
}

void DepthShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the Input Layout
	deviceContext->IASetInputLayout(m_vertLayout);

	//Set Vertex and Pixel Shaders
	deviceContext->VSSetShader(m_VS, 0, 0);
	deviceContext->PSSetShader(m_PS, 0, 0);

	//Draw
	deviceContext->DrawIndexed(indexCount, 0, 0);
}