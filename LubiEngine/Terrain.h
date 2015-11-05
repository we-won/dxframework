#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class Terrain
{
private:
	//Vertex Structure and Vertex Layout (Input Layout)//
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex(){}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz){}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};

	struct Landscape
	{
		int x;
		int y;
	};

public:
	Terrain();
	~Terrain();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ReleaseObjects();
	bool Spin(float time);
	bool Rotate(float time);
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	XMFLOAT4X4 GetWorldMatrix();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11ShaderResourceView* m_texture;

	XMFLOAT4X4 m_world;
	
	Landscape m_terrain;
	int m_vertexCount;
	int m_indexCount;
};

#endif
