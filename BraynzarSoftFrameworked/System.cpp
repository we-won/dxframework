#include "System.h"


System::System()
{
}


System::~System()
{
}

bool System::Initialize(HINSTANCE hInstance)
{
	int width = 800;	//window width
	int height = 600;	//window height

	InitializeWindows(width, height, hInstance);

	_application = new Application;
	if (!_application)
	{
		return false;
	}

	if (!_application->Initialize(hInstance, _hwnd, width, height)){
		return false;
	}

	return true;
}

void System::InitializeWindows(int width, int height, HINSTANCE hInstance)
{
	//Start creating the window//

	WNDCLASSEX wc;	//Create a new extended windows class

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
	wc.lpszClassName	= _wndClassName;	//Name of our windows class
	wc.hIconSm			= LoadIcon(NULL, IDI_WINLOGO); //Icon in your taskbar

	//Register our windows class
	RegisterClassEx(&wc);
	
	//Create our Extended Window
	_hwnd = CreateWindowEx(	NULL,	//Extended style
							_wndClassName,	//Name of our windows class
							"Window Title",	//Name in the title bar of our window
							WS_OVERLAPPEDWINDOW,	//style of our window
							CW_USEDEFAULT, CW_USEDEFAULT,	//Top left corner of window
							width,	//Width of our window
							height,	//Height of our window
							NULL,	//Handle to parent window
							NULL,	//Handle to a Menu
							hInstance,	//Specifies instance of current program
							NULL	//used for an MDI client window
							);
	
	ShowWindow(_hwnd, SW_SHOW);	//Shows our window
	UpdateWindow(_hwnd);	//Its good to update our window
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
		if (!_application->Frame())
		{
			break;
		}

	}

	return (int)msg.wParam;		//return the message

}


LRESULT CALLBACK WndProc(HWND hwnd,	UINT msg, WPARAM wParam, LPARAM lParam) //Default windows procedure
{
	switch (msg)	//Check message
	{

	case WM_KEYDOWN:	//For a key down
		//if escape key was pressed, display popup box
		if (wParam == VK_ESCAPE){
			if (MessageBox(0, "Are you sure you want to exit?",
				"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)

				//Release the windows allocated memory  
				DestroyWindow(hwnd);
		}
		return 0;

	case WM_DESTROY:	//if x button in top right was pressed
		PostQuitMessage(0);
		return 0;
	}
	//return the message for windows to handle it
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void System::Shutdown()
{
	// Release the d3dbase
	if (_application)
	{
		_application->Shutdown();
		delete _application;
	}

	// Shutdown the window.
	ShutdownWindows();

	return;
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
	DestroyWindow(_hwnd);
	_hwnd = NULL;

	// Remove the application instance.
	//UnregisterClass(_wndClassName, m_hinstance);
	//m_hinstance = NULL;

	return;
}