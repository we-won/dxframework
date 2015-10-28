#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class Triangle
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
	Triangle();
	~Triangle();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ReleaseObjects();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

private:
	ID3D11Buffer* _triangleVertBuffer;

	Vertex* _vertices;
	int _indexCount;
};

#endif
