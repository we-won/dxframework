#ifndef _LIGHTLX_H_
#define _LIGHTLX_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class LightLx
{
public:
	LightLx();
	~LightLx();

	bool Initialize(ID3D11Device* device);
	void ReleaseObjects();

	bool GenerateViewMatrix();
	bool GenerateProjectionMatrix();

	XMFLOAT4X4 GetViewMatrix() { return m_viewMatrix; }
	XMFLOAT4X4 GetProjectionMatrix() { return m_projectionMatrix; }

private:
	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;
};

#endif