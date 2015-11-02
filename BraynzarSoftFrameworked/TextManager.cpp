#include "TextManager.h"


TextManager::TextManager()
{
	_vertexBuffer = 0;
	_texture = 0;
	_maxLetters = 0;
	_length = 0;
}


TextManager::~TextManager()
{
}

bool TextManager::Initialize(ID3D11Device* device)
{
	HRESULT hr;

	_maxLetters = 24;

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	const int sizeOfSprite = sizeof(Vertex)* 6;

	vertexBufferDesc.ByteWidth = sizeOfSprite * _maxLetters;

	hr = device->CreateBuffer(&vertexBufferDesc, 0, &_vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	// Create a texture sampler state description.
	hr = D3DX11CreateShaderResourceViewFromFile(device, "font.dds", NULL, NULL, &_texture, NULL);
	if (FAILED(hr))
	{
		MessageBox(0, "Error loading font texture!", "Compile Error", MB_OK);
		return false;
	}

	XMMATRIX world = XMMatrixIdentity();
	XMStoreFloat4x4(&_world, world);

	return true;
}

void TextManager::ReleaseObjects()
{
	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}

	if (_texture)
	{
		_texture->Release();
		_texture = 0;
	}
}

void TextManager::Update(float dt)
{
}

void TextManager::Render(ID3D11DeviceContext* deviceContext, char* message)
{
	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	//Set Primitive Topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DrawString(deviceContext, message, 0.0f, 0.0f);
}

bool TextManager::DrawString(ID3D11DeviceContext* deviceContext, char* message, float startX, float startY)
{
	HRESULT hr;

	// Size in bytes for a single sprite.
	const int sizeOfSprite = sizeof(Vertex)* 6;

	_length = strlen(message);

	// Clamp for strings too long.
	if (_length > _maxLetters)
		_length = _maxLetters;

	// Char's width on screen.
	float charWidth = 32.0f / 800.0f;

	// Char's height on screen.
	float charHeight = 32.0f / 640.0f;

	// Char's texel width.
	float texelWidth = 32.0f / 864.0f;

	// verts per-triangle (3) * total triangles (2) = 6.
	const int verticesPerLetter = 6;

	D3D11_MAPPED_SUBRESOURCE mapResource;
	hr = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	if (FAILED(hr))
	{
		return false;
	}

	// Point to our vertex buffer's internal data.
	Vertex *spritePtr = (Vertex*)mapResource.pData;

	const int indexA = static_cast<char>('A');
	const int indexZ = static_cast<char>('Z');

	for (int i = 0; i < _length; ++i)
	{
		float thisStartX = startX + (charWidth * static_cast<float>(i));
		float thisEndX = thisStartX + charWidth;
		float thisEndY = startY + charHeight;

		spritePtr[0].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);
		spritePtr[1].pos = XMFLOAT3(thisEndX, startY, 1.0f);
		spritePtr[2].pos = XMFLOAT3(thisStartX, startY, 1.0f);
		spritePtr[3].pos = XMFLOAT3(thisStartX, startY, 1.0f);
		spritePtr[4].pos = XMFLOAT3(thisStartX, thisEndY, 1.0f);
		spritePtr[5].pos = XMFLOAT3(thisEndX, thisEndY, 1.0f);

		int texLookup = 0;
		int letter = static_cast<char>(message[i]);

		if (letter < indexA || letter > indexZ)
		{
			// Grab one index past Z, which is a blank space in the texture.
			texLookup = (indexZ - indexA) + 1;
		}
		else
		{
			// A = 0, B = 1, Z = 25, etc.
			texLookup = (letter - indexA);
		}

		float tuStart = 0.0f + (texelWidth * static_cast<float>(texLookup));
		float tuEnd = tuStart + texelWidth;

		spritePtr[0].texCoord = XMFLOAT2(tuEnd, 0.0f);
		spritePtr[1].texCoord = XMFLOAT2(tuEnd, 1.0f);
		spritePtr[2].texCoord = XMFLOAT2(tuStart, 1.0f);
		spritePtr[3].texCoord = XMFLOAT2(tuStart, 1.0f);
		spritePtr[4].texCoord = XMFLOAT2(tuStart, 0.0f);
		spritePtr[5].texCoord = XMFLOAT2(tuEnd, 0.0f);

		spritePtr += 6;
	}

	deviceContext->Unmap(_vertexBuffer, 0);

	return true;
}

int TextManager::GetVertexCount()
{
	return _length * 6;
}

XMFLOAT4X4 TextManager::GetWorldMatrix()
{
	return _world;
}

ID3D11ShaderResourceView* TextManager::GetTexture()
{
	return _texture;
}