#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class Camera
{
public:
	Camera();
	~Camera();

	bool Render();
	XMFLOAT4X4 GetViewMatrix();

private:
	XMFLOAT4 _camPosition;
	XMFLOAT4 _camTarget;
	XMFLOAT4 _camUp;
	XMFLOAT4X4 _view;

};

#endif