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
			float u, float v)
			: pos(x, y, z), texCoord(u, v){}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
	};

public:
	Cube();
	~Cube();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ReleaseObjects();
	bool Spin();
	bool Rotate();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	XMFLOAT4X4 GetWorldMatrix();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11Buffer* _squareIndexBuffer;
	ID3D11Buffer* _squareVertBuffer;
	ID3D11Buffer* cbPerObjectBuffer;

	ID3D11ShaderResourceView* _texture;

	XMFLOAT4X4 _world;

	Vertex* _vertices;
	int _indexCount;
	float _rot;
};

#endif
