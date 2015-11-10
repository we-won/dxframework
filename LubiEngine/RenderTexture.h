#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class RenderTexture
{
public:
	RenderTexture();
	~RenderTexture();

	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight, float screenNear, float screenDepth);
	void ReleaseObjects();

	void SetRenderTarget(ID3D11DeviceContext* deviceContext);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext);
	
	ID3D11ShaderResourceView* GetShaderResourceView() { return m_shaderResourceView; }
	XMFLOAT4X4 GetProjectionMatrix() { return m_projectionMatrix; }
	XMFLOAT4X4 GetOrthoMatrix() { return m_orthoMatrix; }

private:
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;

	XMFLOAT4X4 m_projectionMatrix;
	XMFLOAT4X4 m_orthoMatrix;
};

#endif