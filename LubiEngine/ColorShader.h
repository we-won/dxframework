#ifndef _COLORSHADER_H_
#define _COLORSHADER_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class ColorShader
{
private:
	//Create effects constant buffer's structure//
	struct cbPerObject
	{
		XMFLOAT4X4  WVP;
	};

public:
	ColorShader();
	~ColorShader();

	bool Initialize(ID3D11Device* device);
	void ReleaseObjects();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);

private:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_VS;
	ID3D11PixelShader* m_PS;
	ID3D11InputLayout* m_vertLayout;
	ID3D11Buffer* m_cbPerObjectBuffer;
	cbPerObject m_cbPerObj;
	XMFLOAT4X4 m_WVP;
};

#endif