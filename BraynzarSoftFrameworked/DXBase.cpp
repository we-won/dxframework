#include "DXBase.h"


DXBase::DXBase()
{
	_swapChain = 0;
	_device = 0;
	_deviceContext = 0;
	_renderTargetView = 0;
	_depthStencilView = 0;
	_depthStencilBuffer = 0;
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

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Create the Depth/Stencil View
	_device->CreateTexture2D(&depthStencilDesc, NULL, &_depthStencilBuffer);
	_device->CreateDepthStencilView(_depthStencilBuffer, NULL, &_depthStencilView);

	//Set our Render Target
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	//Set our Render Target
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, NULL);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	//Set the Viewport
	_deviceContext->RSSetViewports(1, &viewport);

	//Set the World matrix
	XMMATRIX world = XMMatrixIdentity();
	XMStoreFloat4x4(&_world, world);

	//Set the Projection matrix
	XMMATRIX camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 1.0f, 1000.0f);
	XMStoreFloat4x4(&_projection, camProjection);

	return true;
}

void DXBase::ReleaseObjects()
{
	//Release the COM Objects we created
	if (_swapChain)
	{
		_swapChain->Release();
		_swapChain = 0;
	}

	if (_device)
	{
		_device->Release();
		_device = 0;
	}

	if (_deviceContext)
	{
		_deviceContext->Release();
		_deviceContext = 0;
	}

	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}
}

bool DXBase::InitScene()
{
	//Clear our backbuffer to the updated color
	float bgColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	_deviceContext->ClearRenderTargetView(_renderTargetView, bgColor);

	//Refresh the Depth/Stencil view
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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

XMFLOAT4X4 DXBase::GetWorldMatrix()
{
	return _world;
}

XMFLOAT4X4 DXBase::GetProjectionMatrix()
{
	return _projection;
}