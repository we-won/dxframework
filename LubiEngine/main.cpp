#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	System* system;
	system = new System;

	if (!system)
	{
		return 0;
	}

	if (system->Initialize(hInstance))
	{
		system->MessageLoop();
	}

	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}
