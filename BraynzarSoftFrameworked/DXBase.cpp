#include "DXBase.h"


DXBase::DXBase()
{
	_swapChain = 0;
	_device = 0;
	_deviceContext = 0;
	_renderTargetView = 0;
	_depthStencilView = 0;
	_depthStencilBuffer = 0;

	_depthStencilState = 0;
	_depthDisabledStencilState = 0;

	_rasterState = 0;
	_noCullRasterState = 0;

	_alphaEnableBlendingState = 0;
	_alphaDisableBlendingState = 0;
}


DXBase::~DXBase()
{
}

bool DXBase::Initialize(HWND hwnd, int width, int height, bool fullscreen)
{
	// Describe our Buffer
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = FALSE;
	}
	else
	{
		swapChainDesc.Windowed = TRUE;
	}

	// Create our SwapChain
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext);

	// Create our BackBuffer
	ID3D11Texture2D* BackBuffer;
	_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	// Create our Render Target
	_device->CreateRenderTargetView(BackBuffer, NULL, &_renderTargetView);
	BackBuffer->Release();

	// Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the Depth/Stencil View
	_device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);

	// Create disabled depth stencil state for 2d objects
	CreateDisabledDepthStencilState();

	// Create default depth stencil state
	CreateDepthStencilState();

	// Set the depth stencil state.
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Initialize the depth stencil view.
	_device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);

	// Set our Render Target
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	// Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// Set the Viewport
	_deviceContext->RSSetViewports(1, &viewport);

	// Create default raster state for opaque objects
	CreateRasterState();

	// Create no cull raster state to render primitives behind alpha
	CreateNoCullRasterState();

	CreateEnabledAlphaBlendingState();

	CreateDisabledAlphaBlendingState();

	// Set the Projection matrix
	XMMATRIX projection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)width / height, 0.1f, 1000.0f);
	XMStoreFloat4x4(&_projection, projection);

	XMMATRIX ortho = XMMatrixOrthographicLH((float)width, (float)height, 0.1f, 1000.0f);
	XMStoreFloat4x4(&_ortho, ortho);

	return true;
}

void DXBase::CreateDepthStencilState()
{
	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	_device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
}

void DXBase::CreateDisabledDepthStencilState()
{
	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	_device->CreateDepthStencilState(&depthDisabledStencilDesc, &_depthDisabledStencilState);
}

void DXBase::CreateRasterState()
{
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	_device->CreateRasterizerState(&rasterDesc, &_rasterState);
}

void DXBase::CreateNoCullRasterState()
{
	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_NONE; // For rending primitives behind alpha
	
	// Create the state using the device.
	_device->CreateRasterizerState(&rasterDesc, &_noCullRasterState);
}

void DXBase::CreateEnabledAlphaBlendingState()
{
	// Clear the blend state description.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	_device->CreateBlendState(&blendStateDescription, &_alphaEnableBlendingState);
}

void DXBase::CreateDisabledAlphaBlendingState()
{
	// Clear the blend state description.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	_device->CreateBlendState(&blendStateDescription, &_alphaDisableBlendingState);
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

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if (_depthDisabledStencilState)
	{
		_depthDisabledStencilState->Release();
		_depthDisabledStencilState = 0;
	}

	if (_rasterState)
	{
		_rasterState->Release();
		_rasterState = 0;
	}

	if (_noCullRasterState)
	{
		_noCullRasterState->Release();
		_noCullRasterState = 0;
	}

	if (_alphaEnableBlendingState)
	{
		_alphaEnableBlendingState->Release();
		_alphaEnableBlendingState = 0;
	}

	if (_alphaDisableBlendingState)
	{
		_alphaDisableBlendingState->Release();
		_alphaDisableBlendingState = 0;
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

void DXBase::TurnZBufferOn()
{
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
}


void DXBase::TurnZBufferOff()
{
	_deviceContext->OMSetDepthStencilState(_depthDisabledStencilState, 1);
}

void DXBase::TurnOffDefaultCulling()
{
	_deviceContext->RSSetState(_noCullRasterState);
}

void DXBase::TurnOnDefaultCulling()
{
	_deviceContext->RSSetState(_rasterState);
}

void DXBase::TurnOnAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	_deviceContext->OMSetBlendState(_alphaEnableBlendingState, blendFactor, 0xffffffff);
}

void DXBase::TurnOffAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	_deviceContext->OMSetBlendState(_alphaDisableBlendingState, blendFactor, 0xffffffff);
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

XMFLOAT4X4 DXBase::GetProjectionMatrix()
{
	return _projection;
}

XMFLOAT4X4 DXBase::GetOrthographicMatrix()
{
	return _ortho;
}