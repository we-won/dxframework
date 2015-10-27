#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "DXBase.h"
#include "Triangle.h"

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
	Triangle* _triangle;
};

#endif
