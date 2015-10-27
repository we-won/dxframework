#include "Triangle.h"


Triangle::Triangle()
{
	_triangleVertBuffer = 0;
	_VS = 0;
	_PS = 0;
	_VS_Buffer = 0;
	_PS_Buffer = 0;
	_vertLayout = 0;

	_vertices = 0;
}


Triangle::~Triangle()
{
}

bool Triangle::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{

	HRESULT hr;
	
	//Compile Shaders from shader file
	hr = D3DX11CompileFromFile("Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &_VS_Buffer, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading vertex shader!", "Compile Error", MB_OK);
		return false;
	}

	hr = D3DX11CompileFromFile("Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &_PS_Buffer, 0, 0);
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

	//Set Vertex and Pixel Shaders
	deviceContext->VSSetShader(_VS, 0, 0);
	deviceContext->PSSetShader(_PS, 0, 0);

	//Create the vertex buffer
	Vertex v[] =
	{
		Vertex(0.0f, 0.5f, 0.5f),
		Vertex(0.5f, -0.5f, 0.5f),
		Vertex(-0.5f, -0.5f, 0.5f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)* 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_triangleVertBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &_triangleVertBuffer, &stride, &offset);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

	//Create the Input Layout
	device->CreateInputLayout(layout, numElements, _VS_Buffer->GetBufferPointer(),
		_VS_Buffer->GetBufferSize(), &_vertLayout);

	//Set the Input Layout
	deviceContext->IASetInputLayout(_vertLayout);

	//Set Primitive Topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	//Set the Viewport
	deviceContext->RSSetViewports(1, &viewport);

	_VS_Buffer->Release();
	_PS_Buffer->Release();

	return true;
}

void Triangle::ReleaseObjects()
{
	_triangleVertBuffer->Release();
	_VS->Release();
	_PS->Release();
	/*_VS_Buffer->Release();
	_PS_Buffer->Release();*/
	_vertLayout->Release();
}

void Triangle::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->Draw(3, 0);
}
