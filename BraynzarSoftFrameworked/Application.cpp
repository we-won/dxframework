#include "Application.h"


Application::Application()
{
	_dxBase = 0;
	_colorShader = 0;
	_textureShader = 0;
	_fontShader = 0;
	_camera = 0;
	_timer = 0;

	_text = 0;
	_cube = 0;
	_cube_2 = 0;
}


Application::~Application()
{
}

bool Application::Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height)
{
	_dxBase = new DXBase;
	if (!_dxBase)
	{
		return false;
	}

	if (!_dxBase->Initialize(hwnd, width, height, FULL_SCREEN))
	{
		return false;
	}

	_colorShader = new ColorShader;
	if (!_colorShader)
	{
		return false;
	}

	if (!_colorShader->Initialize(_dxBase->GetDevice()))
	{
		return false;
	}

	_textureShader = new TextureShader;
	if (!_textureShader)
	{
		return false;
	}

	if (!_textureShader->Initialize(_dxBase->GetDevice()))
	{
		return false;
	}

	_fontShader = new FontShader;
	if (!_fontShader)
	{
		return false;
	}

	if (!_fontShader->Initialize(_dxBase->GetDevice()))
	{
		return false;
	}

	_timer = new TimerLx;
	if (!_timer)
	{
		return false;
	}

	if (!_timer->Initialize())
	{
		return false;
	}

	_text = new TextManager;
	if (!_text)
	{
		return false;
	}

	if (!_text->Initialize(_dxBase->GetDevice()))
	{
		return false;
	}

	_cube = new Cube;
	if (!_cube)
	{
		return false;
	}

	if (!_cube->Initialize(_dxBase->GetDevice(), _dxBase->GetDeviceContext()))
	{
		return false;
	}

	_cube_2 = new Cube;
	if (!_cube_2)
	{
		return false;
	}

	if (!_cube_2->Initialize(_dxBase->GetDevice(), _dxBase->GetDeviceContext()))
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

	if (_textureShader)
	{
		_textureShader->ReleaseObjects();
		delete _textureShader;
		_textureShader = 0;
	}

	if (_fontShader)
	{
		_fontShader->ReleaseObjects();
		delete _fontShader;
		_fontShader = 0;
	}

	if (_timer)
	{
		delete _timer;
		_timer = 0;
	}

	if (_text)
	{
		_text->ReleaseObjects();
		delete _text;
		_text = 0;
	}

	if (_cube)
	{
		_cube->ReleaseObjects();
		delete _cube;
		_cube = 0;
	}

	if (_cube_2)
	{
		_cube_2->ReleaseObjects();
		delete _cube;
		_cube_2 = 0;
	}
}

bool Application::Frame()
{
	bool result;

	_timer->Frame();

	_cube->Spin(_timer->GetFrameTime());
	_cube_2->Rotate(_timer->GetFrameTime());

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
	XMFLOAT4X4 worldMatrix, viewMatrix, projectionMatrix, orthographicMatrix;
	bool result;

	_dxBase->InitScene();
	
	_camera->Render();

	viewMatrix = _camera->GetViewMatrix();
	projectionMatrix = _dxBase->GetProjectionMatrix();
	orthographicMatrix = _dxBase->GetOrthographicMatrix();

	//First Cube
	worldMatrix = _cube->GetWorldMatrix();

	_cube->Render(_dxBase->GetDeviceContext());
	result = _textureShader->Render(_dxBase->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, _cube->GetIndexCount(), _cube->GetTexture());
	if (!result)
	{
		return false;
	}

	//Second Cube
	worldMatrix = _cube_2->GetWorldMatrix();

	_cube_2->Render(_dxBase->GetDeviceContext());
	result = _textureShader->Render(_dxBase->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, _cube_2->GetIndexCount(), _cube_2->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	_dxBase->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	_dxBase->TurnOnAlphaBlending();

	//Text
	worldMatrix = _text->GetWorldMatrix();

	_text->Render(_dxBase->GetDeviceContext(), "HELLO");
	result = _fontShader->Render(_dxBase->GetDeviceContext(), _text->GetVertexCount(), _text->GetWorldMatrix(), viewMatrix, orthographicMatrix, _text->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	_dxBase->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	_dxBase->TurnZBufferOn();
	
	_dxBase->Present();

	return true;
}