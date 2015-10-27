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
		Vertex(float x, float y, float z)
			: pos(x, y, z){}

		XMFLOAT3 pos;
	};

public:
	Triangle();
	~Triangle();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void ReleaseObjects();

	void Render(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* _triangleVertBuffer;
	ID3D11VertexShader* _VS;
	ID3D11PixelShader* _PS;
	ID3D10Blob* _VS_Buffer;
	ID3D10Blob* _PS_Buffer;
	ID3D11InputLayout* _vertLayout;

	Vertex* _vertices;
};

#endif
