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
	DXBase* m_dxBase;
	ColorShader* m_colorShader;
	TextureShader* m_textureShader;
	FontShader* m_fontShader;
	Camera* m_camera;
	TimerLx* m_timer;
	InputLx* m_input;

	//Displays
	TextManager* m_text;
	Cube* m_cube;
	Cube* m_cube_2;
};

#endif
