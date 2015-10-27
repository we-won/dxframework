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

	bool Initialize(HWND hwnd, int width, int height);
	void ReleaseObjects();
	bool InitScene();
	void Present();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	
private:
	HINSTANCE _hInstance;

	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _renderTargetView;

	float _red, _green, _blue;
	int _colormodr, _colormodg, _colormodb;
};

#endif