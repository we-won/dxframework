#include "Cube.h"


Cube::Cube()
{
	_squareIndexBuffer = 0;
	_squareVertBuffer = 0;
	_vertices = 0;
	_texture = 0;

	_rot = 0.01f;
}


Cube::~Cube()
{
}

bool Cube::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;

	_indexCount = 36;

	//Create the vertex buffer
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-1.0f, 1.0f, 1.0f, 1.0f, 0.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 0.0f),

		// Left Face
		Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
		Vertex(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
		Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
		Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0, 1, 2,
		0, 2, 3,

		// Back Face
		4, 5, 6,
		4, 6, 7,

		// Top Face
		8, 9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	device->CreateBuffer(&indexBufferDesc, &iinitData, &_squareIndexBuffer);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 24;
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

	// Create a texture sampler state description.
	hr = D3DX11CreateShaderResourceViewFromFile(device, "braynzar.jpg", NULL, NULL, &_texture, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	//Set the World matrix
	XMMATRIX world = XMMatrixIdentity();
	XMStoreFloat4x4(&_world, world);

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

	if (_texture)
	{
		_texture->Release();
		_texture = 0;
	}
}

bool Cube::Spin()
{
	//Keep the cubes rotating
	_rot += .0005f;
	if (_rot > 6.26f)
		_rot = 0.0f;

	//Define cube1's world space matrix
	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, _rot);
	XMMATRIX Translation = XMMatrixTranslation(0.0f, 0.0f, 4.0f);

	//Set cube1's world space using the transformations
	XMMATRIX cubeWorld = Translation * Rotation;
	XMStoreFloat4x4(&_world, cubeWorld);

	return true;
}

bool Cube::Rotate()
{
	//Keep the cubes rotating
	_rot += .0005f;
	if (_rot > 6.26f)
		_rot = 0.0f;

	//Define cube2's world space matrix
	XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX Rotation = XMMatrixRotationAxis(rotaxis, -_rot);
	XMMATRIX Scale = XMMatrixScaling(1.3f, 1.3f, 1.3f);

	//Set cube2's world space matrix
	XMMATRIX cubeWorld = Rotation * Scale;
	XMStoreFloat4x4(&_world, cubeWorld);

	return true;
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

XMFLOAT4X4 Cube::GetWorldMatrix()
{
	return _world;
}

ID3D11ShaderResourceView* Cube::GetTexture()
{
	return _texture;
}

