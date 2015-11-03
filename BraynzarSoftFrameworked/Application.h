#ifndef _APPLICATION_H_
#define _APPLICATION_H_

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;

#include "DXBase.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "FontShader.h"
#include "Camera.h"
#include "TimerLx.h"
#include "InputLx.h"

#include "TextManager.h"
#include "Cube.h"

class Application
{
public:
	Application();
	~Application();

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height);
	void Shutdown();
	bool Frame();

	void HandleInput(LPARAM lParam);

private:
	bool RenderGraphics();

private:
	DXBase* _dxBase;
	ColorShader* _colorShader;
	TextureShader* _textureShader;
	FontShader* _fontShader;
	Camera* _camera;
	TimerLx* _timer;
	InputLx* _input;

	//Displays
	TextManager* _text;
	Cube* _cube;
	Cube* _cube_2;
};

#endif
