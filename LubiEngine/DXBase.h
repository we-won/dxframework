#ifndef _DXBASE_H_
#define _DXBASE_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class DXBase
{
public:
	DXBase();
	~DXBase();

	bool Initialize(HWND hwnd, int width, int height, bool fullscreen, bool vsync_enabled);
	void ReleaseObjects();
	bool InitScene();
	void Present();

	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnOnDefaultCulling();
	void TurnOffDefaultCulling();
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	ID3D11Device* GetDevice() { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }
	
	XMFLOAT4X4 GetWorldMatrix() { return m_world; }
	XMFLOAT4X4 GetProjectionMatrix() { return m_projection; }
	XMFLOAT4X4 GetOrthographicMatrix() { return m_ortho; }

private:
	void CreateDepthStencilState();
	void CreateDisabledDepthStencilState();
	void CreateRasterState();
	void CreateNoCullRasterState();
	void CreateEnabledAlphaBlendingState();
	void CreateDisabledAlphaBlendingState();

private:
	bool m_vsync_enabled;

	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_noCullRasterState;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;

	XMFLOAT4X4 m_world;
	XMFLOAT4X4 m_projection;
	XMFLOAT4X4 m_ortho;
};

#endif