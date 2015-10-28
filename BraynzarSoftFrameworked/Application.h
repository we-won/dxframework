#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "DXBase.h"
#include "ColorShader.h"
#include "Camera.h"

#include "Triangle.h"
#include "Cube.h"

class Application
{
public:
	Application();
	~Application();

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height);
	void Shutdown();
	bool Frame();

private:
	bool RenderGraphics();

private:
	DXBase* _dxBase;
	ColorShader* _colorShader;
	Camera* _camera;

	//Displays
	Triangle* _triangle;
	Cube* _cube;
};

#endif
