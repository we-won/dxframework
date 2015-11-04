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

	int GetVertexCount() { return m_length * 6; }
	XMFLOAT4X4 GetWorldMatrix() { return m_world; }
	ID3D11ShaderResourceView* GetTexture() { return m_texture; }

private:
	bool DrawString(ID3D11DeviceContext* deviceContext, char* message, float startX, float startY);

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11ShaderResourceView* m_texture;

	XMFLOAT4X4 m_world;

	int m_maxLetters;
	int m_length;
};

#endif