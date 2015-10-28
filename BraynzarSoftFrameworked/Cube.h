#ifndef _CUBE_H_
#define _CUBE_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class Cube
{
private:
	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex(){}
		Vertex(float x, float y, float z,
			float cr, float cg, float cb, float ca)
			: pos(x, y, z), color(cr, cg, cb, ca){}

		XMFLOAT3 pos;
		XMFLOAT4 color;
	};

public:
	Cube();
	~Cube();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ReleaseObjects();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	ID3D11Buffer* _squareIndexBuffer;
	ID3D11Buffer* _squareVertBuffer;
	ID3D11Buffer* cbPerObjectBuffer;

	Vertex* _vertices;
	int _indexCount;
};

#endif
