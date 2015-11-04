#include "Triangle.h"


Triangle::Triangle()
{
	_triangleVertBuffer = 0;
	_vertices = 0;
}


Triangle::~Triangle()
{
}

bool Triangle::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;

	_indexCount = 3;

	//Create the vertex buffer
	Vertex v[] =
	{
		Vertex(0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex)* _indexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_triangleVertBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void Triangle::ReleaseObjects()
{
	if (_triangleVertBuffer)
	{
		_triangleVertBuffer->Release();
		_triangleVertBuffer = 0;
	}
	
}

void Triangle::Render(ID3D11DeviceContext* deviceContext)
{
	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &_triangleVertBuffer, &stride, &offset);

	//Set Primitive Topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Triangle::GetIndexCount()
{
	return _indexCount;
}
