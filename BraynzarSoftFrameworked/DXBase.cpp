#include "DXBase.h"


DXBase::DXBase()
{
	_red = 0.0f;
	_green = 0.0f;
	_blue = 0.0f;
	_colormodr = 1;
	_colormodg = 1;
	_colormodb = 1;
}


DXBase::~DXBase()
{
}

bool DXBase::Initialize(HWND hwnd, int width, int height)
{
	//Describe our Buffer
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//Create our SwapChain
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext);

	//Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//Create our Render Target
	_device->CreateRenderTargetView(BackBuffer, NULL, &_renderTargetView);
	BackBuffer->Release();

	//Set our Render Target
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, NULL);

	return true;
}

void DXBase::ReleaseObjects()
{
	//Release the COM Objects we created
	_swapChain->Release();
	_device->Release();
	_deviceContext->Release();
}

bool DXBase::InitScene()
{
	//Clear our backbuffer to the updated color
	float bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	_deviceContext->ClearRenderTargetView(_renderTargetView, bgColor);

	return true;
}

void DXBase::Present()
{
	//Present the backbuffer to the screen
	_swapChain->Present(0, 0);
}

ID3D11Device* DXBase::GetDevice()
{
	return _device;
}

ID3D11DeviceContext* DXBase::GetDeviceContext()
{
	return _deviceContext;
}