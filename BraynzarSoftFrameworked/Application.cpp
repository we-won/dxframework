#include "Application.h"


Application::Application()
{
	_dxBase = 0;
	_colorShader = 0;
	_camera = 0;
	
	_triangle = 0;
	_cube = 0;
}


Application::~Application()
{
}

bool Application::Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height)
{
	_dxBase = new DXBase;

	if (!_dxBase->Initialize(hwnd, width, height))
	{
		return false;
	}

	_colorShader = new ColorShader;

	if (!_colorShader->Initialize(_dxBase->GetDevice()))
	{
		return false;
	}

	_triangle = new Triangle;

	if (!_triangle->Initialize(_dxBase->GetDevice(), _dxBase->GetDeviceContext()))
	{
		return false;
	}

	_cube = new Cube;

	if (!_cube->Initialize(_dxBase->GetDevice(), _dxBase->GetDeviceContext()))
	{
		return false;
	}

	_camera = new Camera;
	if (!_camera)
	{
		return false;
	}

	return true;
}

void Application::Shutdown()
{
	if (_dxBase)
	{
		_dxBase->ReleaseObjects();
		delete _dxBase;
		_dxBase = 0;
	}

	if (_colorShader)
	{
		_colorShader->ReleaseObjects();
		delete _colorShader;
		_colorShader = 0;
	}

	if (_triangle)
	{
		_triangle->ReleaseObjects();
		delete _triangle;
		_triangle = 0;
	}

	if (_cube)
	{
		_cube->ReleaseObjects();
		delete _cube;
		_cube = 0;
	}
}

bool Application::Frame()
{
	bool result;

	// Render the graphics.
	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}

bool Application::RenderGraphics()
{
	XMFLOAT4X4 worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	_dxBase->InitScene();
	
	_camera->Render();

	worldMatrix = _dxBase->GetWorldMatrix();
	viewMatrix = _camera->GetViewMatrix();
	projectionMatrix = _dxBase->GetProjectionMatrix();

	//_triangle->Render(_dxBase->GetDeviceContext());
	_cube->Render(_dxBase->GetDeviceContext());

	result = _colorShader->Render(_dxBase->GetDeviceContext(), _cube->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	_dxBase->Present();

	return true;
}