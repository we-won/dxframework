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

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT4X4 GetOrthographicMatrix();

private:
	void CreateDepthStencilState();
	void CreateDisabledDepthStencilState();
	void CreateRasterState();
	void CreateNoCullRasterState();
	void CreateEnabledAlphaBlendingState();
	void CreateDisabledAlphaBlendingState();

private:
	bool _vsync_enabled;
	HINSTANCE _hInstance;
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11DepthStencilState* _depthDisabledStencilState;
	ID3D11RasterizerState* _rasterState;
	ID3D11RasterizerState* _noCullRasterState;
	ID3D11BlendState* _alphaEnableBlendingState;
	ID3D11BlendState* _alphaDisableBlendingState;

	XMFLOAT4X4 _projection;
	XMFLOAT4X4 _ortho;

	float _red, _green, _blue;
	int _colormodr, _colormodg, _colormodb;
};

#endif