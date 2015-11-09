#include "Terrain.h"


Terrain::Terrain()
: m_indexBuffer(0), m_vertexBuffer(0), m_texture(0)
{
	m_terrain.x = 50;
	m_terrain.y = 50;

	m_vertexCount = m_terrain.x * m_terrain.y * 4;
	m_indexCount = m_terrain.x * m_terrain.y * 6;
}


Terrain::~Terrain()
{
}

bool Terrain::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr;

	Vertex* vertices = new Vertex[m_vertexCount];
	DWORD* indices = new DWORD[m_indexCount];


	int index_v = 0;
	int index_i = 0;

	for( int x = 0; x < m_terrain.x; ++x )
	{
		for( int y = 0; y < m_terrain.y; ++y )
		{
			// lower left
			vertices[index_v] = Vertex(0.0f + x, -2.0f, 0.0f + y, 0.0f, 1.0f, -1.0f, -1.0f, -1.0f);
			indices[index_i] = index_v;
			indices[index_i + 3] = index_v;
			++index_v;

			// lower right
			vertices[index_v] = Vertex(0.0f + x, -2.0f, 1.0f + y, 0.0f, 0.0f, -1.0f, +1.0f, -1.0f);
			indices[index_i + 1] = index_v;
			++index_v;

			// upper right
			vertices[index_v] = Vertex(1.0f + x, -2.0f, 1.0f + y, 1.0f, 0.0f, +1.0f, +1.0f, -1.0f);
			indices[index_i + 2] = index_v;
			indices[index_i + 4] = index_v;
			++index_v;

			// upper left
			vertices[index_v] = Vertex(1.0f + x, -2.0f, 0.0f + y, 1.0f, 1.0f, +1.0f, -1.0f, -1.0f);
			indices[index_i + 5] = index_v;
			++index_v;

			index_i += 6;
		}
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	device->CreateBuffer(&indexBufferDesc, &iinitData, &m_indexBuffer);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertices;
	hr = device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// Create a texture sampler state description.
	hr = D3DX11CreateShaderResourceViewFromFile(device, "grass.jpg", NULL, NULL, &m_texture, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	//Set the World matrix
	XMMATRIX world = XMMatrixIdentity();
	XMStoreFloat4x4(&m_world, world);

	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

void Terrain::ReleaseObjects()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}

void Terrain::Render(ID3D11DeviceContext* deviceContext)
{
	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//Set Primitive Topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

int Terrain::GetIndexCount()
{
	return m_indexCount;
}

XMFLOAT4X4 Terrain::GetWorldMatrix()
{
	return m_world;
}

ID3D11ShaderResourceView* Terrain::GetTexture()
{
	return m_texture;
}

