#ifndef _FONTSHADER_H_
#define _FONTSHADER_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class FontShader
{
private:
	struct cbPerObject
	{
		XMFLOAT4X4  WVP;
	};

public:
	FontShader();
	~FontShader();

	bool Initialize(ID3D11Device* device);
	void ReleaseObjects();
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 orthoMatrix, ID3D11ShaderResourceView* texture);

private:
	bool FontShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMFLOAT4X4 worldMatrix, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 orthoMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_VS;
	ID3D11PixelShader* m_PS;
	ID3D11InputLayout* m_vertLayout;
	ID3D11Buffer* m_cbPerObjectBuffer;
	ID3D11SamplerState* m_texSamplerState;
	cbPerObject m_cbPerObj;
};

#endif
