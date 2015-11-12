#ifndef _MODELLX_H_
#define _MODELLX_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <fstream>

using namespace std;

class ModelLx
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelLx();
	~ModelLx();

	bool Initialize(ID3D11Device* device, char* modelFilename, LPCTSTR textureFilename);
	void ReleaseObjects();
	void Render(ID3D11DeviceContext* deviceContext);
	
	void SetPosition(float x, float y, float z);
	void GetPosition(float& x, float& y, float& z);

	int GetIndexCount() { return m_indexCount; }
	ID3D11ShaderResourceView* GetTexture() { return m_texture;}
		
private:
	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	bool LoadTexture(ID3D11Device* device, LPCTSTR textureFilename);
	bool LoadModel(char* modelFilename);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ID3D11ShaderResourceView* m_texture;
	ModelType* m_model;
	float m_positionX, m_positionY, m_positionZ;
};

#endif
