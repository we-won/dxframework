#include "Cube.h"


Cube::Cube()
{
	_squareIndexBuffer = 0;
	_squareVertBuffer = 0;
	_vertices = 0;
}


Cube::~Cube()
{
}

bool Cube::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;

	_indexCount = 6;

	//Create the vertex buffer
	Vertex v[] =
	{
		Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex(0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
	};

	DWORD indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD)* 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	device->CreateBuffer(&indexBufferDesc, &iinitData, &_squareIndexBuffer);

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
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &_squareVertBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}

void Cube::ReleaseObjects()
{
	if (_squareIndexBuffer)
	{
		_squareIndexBuffer->Release();
		_squareIndexBuffer = 0;
	}

	if (_squareVertBuffer)
	{
		_squareVertBuffer->Release();
		_squareVertBuffer = 0;
	}

}

void Cube::Render(ID3D11DeviceContext* deviceContext)
{
	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetIndexBuffer(_squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &_squareVertBuffer, &stride, &offset);

	//Set Primitive Topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Cube::GetIndexCount()
{
	return _indexCount;
}
