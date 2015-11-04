#include "System.h"


System::System()
	: m_application(0)
{
	m_wndTitle = "Lubi Engine";
	m_wndClassName = "lubiengine";
	m_hwnd = NULL;
}


System::~System()
{
}

bool System::Initialize(HINSTANCE hInstance)
{
	int width = 0;	//window width
	int height = 0;	//window height

	InitializeWindows(width, height, hInstance);

	m_application = new Application;
	if (!m_application)
	{
		return false;
	}

	if (!m_application->Initialize(hInstance, m_hwnd, width, height)){
		return false;
	}

	return true;
}

void System::InitializeWindows(int& width, int& height, HINSTANCE hInstance)
{
	//Start creating the window//

	WNDCLASSEX wc;	//Create a new extended windows class
	DEVMODE dmScreenSettings;
	int posX, posY;

	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Setup the windows class with default settings.
	wc.cbSize			= sizeof(WNDCLASSEX);	//Size of our windows class
	wc.style			= CS_HREDRAW | CS_VREDRAW;	//class styles
	wc.lpfnWndProc		= WndProc;	//Default windows procedure function
	wc.cbClsExtra		= NULL;	//Extra bytes after our wc structure
	wc.cbWndExtra		= NULL;	//Extra bytes after our windows instance
	wc.hInstance		= hInstance;	//Instance to current application
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);	//Title bar Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);	//Default mouse Icon
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 2);	//Window bg color
	wc.lpszMenuName		= NULL;	//Name of the menu attached to our window
	wc.lpszClassName	= m_wndClassName;	//Name of our windows class
	wc.hIconSm			= LoadIcon(NULL, IDI_WINLOGO); //Icon in your taskbar

	//Register our windows class
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	
	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)width;
		dmScreenSettings.dmPelsHeight = (unsigned long)height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		width = 800;
		height = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	//Create our Extended Window
	m_hwnd = CreateWindowEx(	NULL,	//Extended style
							m_wndClassName,	//Name of our windows class
							m_wndTitle,	//Name in the title bar of our window
							WS_OVERLAPPEDWINDOW,	//style of our window
							posX, posY,	//Center of our window
							width,	//Width of our window
							height,	//Height of our window
							NULL,	//Handle to parent window
							NULL,	//Handle to a Menu
							hInstance,	//Specifies instance of current program
							NULL	//used for an MDI client window
							);
	
	ShowWindow(m_hwnd, SW_SHOW);	//Shows our window
	UpdateWindow(m_hwnd);	//Its good to update our window
}

int System::MessageLoop(){	//The message loop

	MSG msg;	//Create a new message structure
	ZeroMemory(&msg, sizeof(MSG));	//clear message structure to NULL

	while (true)	//while there is a message
	{
		//if there was a windows message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)	//if the message was WM_QUIT
				break;	//Exit the message loop

			TranslateMessage(&msg);	//Translate the message

			//Send the message to default windows procedure
			DispatchMessage(&msg);
		}

		//run
		if (!m_application->Frame())
		{
			break;
		}

	}

	return (int)msg.wParam;		//return the message

}

LRESULT CALLBACK System::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//return the message for windows to handle it
	switch (msg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				//Release the windows allocated memory  
				DestroyWindow(hwnd);
			}
			return 0;

		case WM_INPUT:
			m_application->HandleInput(lParam);
			return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,	UINT msg, WPARAM wParam, LPARAM lParam) //Default windows procedure
{
	return ApplicationHandle->MessageHandler(hwnd, msg, wParam, lParam);
}

void System::Shutdown()
{
	// Release the d3dbase
	if (m_application)
	{
		m_application->Shutdown();
		delete m_application;
	}

	// Shutdown the window.
	ShutdownWindows();
}

void System::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
}