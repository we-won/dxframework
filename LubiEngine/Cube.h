#ifndef _CUBE_H_
#define _CUBE_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class Cube
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

public:
	Cube();
	~Cube();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ReleaseObjects();
	void Render(ID3D11DeviceContext* deviceContext);

	void MoveTo(float x, float z);
	bool Spin(float time);
	bool Rotate(float time);

	int GetIndexCount();
	XMFLOAT4X4 GetWorldMatrix();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11Buffer* _squareIndexBuffer;
	ID3D11Buffer* _squareVertBuffer;

	ID3D11ShaderResourceView* _texture;

	XMFLOAT4X4 _world;

	Vertex* _vertices;
	int _indexCount;
	float _rot;
};

#endif
