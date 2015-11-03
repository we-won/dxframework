#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//Include necessary Headers//
#include <windows.h>

#include "Application.h"

class System
{
public:
	System();
	~System();

	//Functions//
	bool Initialize(HINSTANCE hInstance); //Initialize
	void InitializeWindows(int& width, int& height, HINSTANCE hInstance); //Initialize our window

	int MessageLoop();	//Main part of the program
	void Shutdown();
	void ShutdownWindows(); 

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	LPCTSTR _wndClassName;
	HWND _hwnd;

	Application* _application;
};

LRESULT CALLBACK WndProc(HWND hWnd,	UINT msg, WPARAM wParam, LPARAM lParam); //Windows callback procedure

static System* ApplicationHandle = 0;

#endif