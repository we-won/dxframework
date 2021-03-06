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
#include "LightLx.h"
#include "RenderTexture.h"
#include "DepthShader.h"

#include "ModelLx.h"
#include "Terrain.h"
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
	bool RenderSceneToTexture();
	bool RenderGraphics();

private:
	float m_width, m_height;
	DXBase* m_dxBase;
	ColorShader* m_colorShader;
	TextureShader* m_textureShader;
	FontShader* m_fontShader;
	Camera* m_camera;
	TimerLx* m_timer;
	InputLx* m_input;
	LightLx* m_light;
	RenderTexture* m_renderTexture;
	DepthShader* m_depthShader;

	XMFLOAT4X4 m_worldMatrix;
	XMFLOAT4X4 m_viewMatrix; 
	XMFLOAT4X4 m_projectionMatrix;
	XMFLOAT4X4 m_orthographicMatrix;

	//Displays
	ModelLx *m_CubeModel, *m_GroundModel, *m_SphereModel; 
	//Terrain* m_terrain;
	//TextManager* m_text;
	//Cube* m_cube;
	//Cube* m_cube_2;

	//Mouse Test
	int m_rightClickState;
};

#endif
