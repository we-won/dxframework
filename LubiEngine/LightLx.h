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

private:
	ID3D11Buffer* m_cbPerFrameBuffer;
	Light m_light;
	cbPerFrame m_constbuffPerFrame;
};

#endif