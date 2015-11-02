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
	ID3D11VertexShader* _VS;
	ID3D11PixelShader* _PS;
	ID3D11InputLayout* _vertLayout;
	ID3D11Buffer* _cbPerObjectBuffer;
	ID3D11SamplerState* _texSamplerState;
	cbPerObject _cbPerObj;
	XMFLOAT4X4 _WVP;
};

#endif
