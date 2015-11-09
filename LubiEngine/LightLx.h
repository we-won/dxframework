#ifndef _LIGHTLX_H_
#define _LIGHTLX_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class LightLx
{
private:
	struct Light
	{
		Light()
		{
			ZeroMemory(this, sizeof(Light));
		}
		XMFLOAT3 dir;
		float pad;
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
	};

	struct cbPerFrame
	{
		Light light;
	};

public:
	LightLx();
	~LightLx();

	bool Initialize(ID3D11Device* device);
	void ReleaseObjects();

	void SetLightEffectsOn();
	void SetLightEffectsOff();
	void ApplyLight(ID3D11DeviceContext* deviceContext);

	bool GenerateViewMatrix();
	bool GenerateProjectionMatrix();

	XMFLOAT4X4 GetViewMatrix() { return m_viewMatrix; }
	XMFLOAT4X4 GetProjectionMatrix() { return m_projectionMatrix; }

private:
	ID3D11Buffer* m_cbPerFrameBuffer;
	Light m_light;
	cbPerFrame m_constbuffPerFrame;

	XMFLOAT4 m_lookAt;
	XMFLOAT4 m_up;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;
};

#endif