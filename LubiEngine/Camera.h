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

	void MoveLeft(float dt);
	void MoveRight(float dt);
	void MoveUp(float dt);
	void MoveDown(float dt);

	bool Render();

	XMFLOAT4X4 GetViewMatrix() { return m_view; }

	float GetCamPosX() { return m_camPosition.x; }
	float GetCamPosZ() { return m_camPosition.z; }

private:
	XMFLOAT4 m_camPosition;
	XMFLOAT4 m_camTarget;
	XMFLOAT4 m_camUp;
	XMFLOAT4X4 m_view;
};

#endif