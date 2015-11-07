#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class TextureShader
{
private:
	//Create effects constant buffer's structure//
	struct cbPerObject
	{
		XMFLOAT4X4  WVP;
		XMFLOAT4X4  World;
	};


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
	TextureShader();
	~TextureShader();

	bool Initialize(ID3D11Device* device);
	void ReleaseObjects();
	bool Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, int indexCount, ID3D11ShaderResourceView* texture);

private:
	bool TextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_VS;
	ID3D11PixelShader* m_PS;
	ID3D11InputLayout* m_vertLayout;
	ID3D11Buffer* m_cbPerObjectBuffer;
	ID3D11SamplerState* m_texSamplerState;
	cbPerObject m_cbPerObj;

	ID3D11Buffer* m_cbPerFrameBuffer;
	Light m_light;
	cbPerFrame m_constbuffPerFrame;
};

#endif
