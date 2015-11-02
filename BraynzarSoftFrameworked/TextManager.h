#ifndef _TEXTMANAGER_H_
#define _TEXTMANAGER_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class TextManager
{
private:
	//Vertex Structure and Vertex Layout (Input Layout)//
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex(){}
		Vertex(float x, float y, float z,
			float u, float v)
			: pos(x, y, z), texCoord(u, v){}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
	};

public:
	TextManager();
	~TextManager();

	bool Initialize(ID3D11Device* device);
	void ReleaseObjects();

	void Update(float dt);
	void Render(ID3D11DeviceContext* deviceContext, char* message);

	int GetVertexCount();
	XMFLOAT4X4 GetWorldMatrix();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool DrawString(ID3D11DeviceContext* deviceContext, char* message, float startX, float startY);

private:
	ID3D11Buffer* _vertexBuffer;
	ID3D11ShaderResourceView* _texture;

	XMFLOAT4X4 _world;

	int _maxLetters;
	int _length;
};

#endif