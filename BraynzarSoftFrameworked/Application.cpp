#include "Application.h"


Application::Application()
{
	_dxBase = 0;
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

	_triangle = new Triangle;

	if (!_triangle->Initialize(_dxBase->GetDevice(), _dxBase->GetDeviceContext()))
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

	if (_triangle)
	{
		_triangle->ReleaseObjects();
		delete _triangle;
		_triangle = 0;
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
	_dxBase->InitScene();

	_triangle->Render(_dxBase->GetDeviceContext());

	_dxBase->Present();

	return true;
}