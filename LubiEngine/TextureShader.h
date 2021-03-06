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
		XMFLOAT4X4 WVP;
		XMFLOAT4X4 World;
		XMFLOAT4X4 LightWVP;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
	};

	struct LightBufferType2
	{
		XMFLOAT3 lightPosition;
		float padding;
	};

public:
	TextureShader();
	~TextureShader();

	bool Initialize(ID3D11Device* device);
	void ReleaseObjects();
	bool Render(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, 
				int indexCount, ID3D11ShaderResourceView* texture, XMFLOAT4X4 lightViewMatrix, XMFLOAT4X4 lightProjectionMatrix, 
				ID3D11ShaderResourceView* depthMapTexture);

private:
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix,
							XMFLOAT4X4 projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4X4 lightViewMatrix, 
							XMFLOAT4X4 lightProjectionMatrix, ID3D11ShaderResourceView* depthMapTexture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_VS;
	ID3D11PixelShader* m_PS;
	ID3D11InputLayout* m_vertLayout;

	ID3D11SamplerState* m_texSamplerState;

	ID3D11Buffer* m_cbPerObjectBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_lightBuffer2;
};

#endif
