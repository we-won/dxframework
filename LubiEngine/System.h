#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <Windows.h>

#include "Application.h"

class System
{
public:
	System();
	~System();

	bool Initialize(HINSTANCE hInstance); //Initialize
	void InitializeWindows(int& width, int& height, HINSTANCE hInstance); //Initialize our window

	int MessageLoop(); //Main part of the program
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void ShutdownWindows(); 

private:
	LPCTSTR m_wndTitle;
	LPCTSTR m_wndClassName;
	HWND m_hwnd;

	Application* m_application;
};

LRESULT CALLBACK WndProc(HWND hWnd,	UINT msg, WPARAM wParam, LPARAM lParam); //Windows callback procedure

static System* ApplicationHandle = 0;

#endif