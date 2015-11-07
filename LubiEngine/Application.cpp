#include "Application.h"


Application::Application()
	: m_dxBase(0), m_colorShader(0), m_textureShader(0), m_fontShader(0), m_camera(0), m_timer(0), m_input(0),
		m_terrain(0), m_text(0), m_cube(0), m_cube_2(0),
		m_rightClickState(0)
{
}


Application::~Application()
{
}

bool Application::Initialize(HINSTANCE hInstance, HWND hwnd, int width, int height)
{
	m_width = (float)width;
	m_height = (float)height;

	m_dxBase = new DXBase;
	if (!m_dxBase)
	{
		return false;
	}

	if (!m_dxBase->Initialize(hwnd, width, height, FULL_SCREEN, VSYNC_ENABLED))
	{
		return false;
	}

	m_colorShader = new ColorShader;
	if (!m_colorShader)
	{
		return false;
	}

	if (!m_colorShader->Initialize(m_dxBase->GetDevice()))
	{
		return false;
	}

	m_textureShader = new TextureShader;
	if (!m_textureShader)
	{
		return false;
	}

	if (!m_textureShader->Initialize(m_dxBase->GetDevice()))
	{
		return false;
	}

	m_fontShader = new FontShader;
	if (!m_fontShader)
	{
		return false;
	}

	if (!m_fontShader->Initialize(m_dxBase->GetDevice()))
	{
		return false;
	}

	m_timer = new TimerLx;
	if (!m_timer)
	{
		return false;
	}

	if (!m_timer->Initialize())
	{
		return false;
	}

	m_input = new InputLx;
	if (!m_input)
	{
		return false;
	}

	if (!m_input->Initialize(hwnd, width, height))
	{
		return false;
	}

	m_terrain = new Terrain;
	if (!m_terrain)
	{
		return false;
	}

	if (!m_terrain->Initialize(m_dxBase->GetDevice(), m_dxBase->GetDeviceContext()))
	{
		return false;
	}

	m_text = new TextManager;
	if (!m_text)
	{
		return false;
	}

	if (!m_text->Initialize(m_dxBase->GetDevice()))
	{
		return false;
	}

	m_cube = new Cube;
	if (!m_cube)
	{
		return false;
	}

	if (!m_cube->Initialize(m_dxBase->GetDevice(), m_dxBase->GetDeviceContext()))
	{
		return false;
	}

	m_cube_2 = new Cube;
	if (!m_cube_2)
	{
		return false;
	}

	if (!m_cube_2->Initialize(m_dxBase->GetDevice(), m_dxBase->GetDeviceContext()))
	{
		return false;
	}

	m_camera = new Camera;
	if (!m_camera)
	{
		return false;
	}

	if (!m_camera->Render())
	{
		return false;
	}

	return true;
}

void Application::Shutdown()
{
	if (m_dxBase)
	{
		m_dxBase->ReleaseObjects();
		delete m_dxBase;
		m_dxBase = 0;
	}

	if (m_colorShader)
	{
		m_colorShader->ReleaseObjects();
		delete m_colorShader;
		m_colorShader = 0;
	}

	if (m_textureShader)
	{
		m_textureShader->ReleaseObjects();
		delete m_textureShader;
		m_textureShader = 0;
	}

	if (m_fontShader)
	{
		m_fontShader->ReleaseObjects();
		delete m_fontShader;
		m_fontShader = 0;
	}

	if (m_timer)
	{
		delete m_timer;
		m_timer = 0;
	}

	if (m_input)
	{
		delete m_input;
		m_input = 0;
	}

	if (m_terrain)
	{
		m_terrain->ReleaseObjects();
		delete m_terrain;
		m_terrain = 0;
	}

	if (m_text)
	{
		m_text->ReleaseObjects();
		delete m_text;
		m_text = 0;
	}

	if (m_cube)
	{
		m_cube->ReleaseObjects();
		delete m_cube;
		m_cube = 0;
	}

	if (m_cube_2)
	{
		m_cube_2->ReleaseObjects();
		delete m_cube;
		m_cube_2 = 0;
	}
}

void Application::HandleInput(LPARAM lParam)
{
	m_input->GetData(lParam);
}

bool Application::Frame()
{
	bool result;

	// Get global matrices
	m_worldMatrix = m_dxBase->GetWorldMatrix();
	m_viewMatrix = m_camera->GetViewMatrix();
	m_projectionMatrix = m_dxBase->GetProjectionMatrix();
	m_orthographicMatrix = m_dxBase->GetOrthographicMatrix();

	m_timer->Frame();
	
	if (m_input->IsArrowKeyDown(VKey_LeftArrow - 0x25)) 
	{
		m_camera->MoveLeft(m_timer->GetTime());
	}

	if (m_input->IsArrowKeyDown(VKey_RightArrow - 0x25)) 
	{
		m_camera->MoveRight(m_timer->GetTime());
	}

	if (m_input->IsArrowKeyDown(VKey_UpArrow - 0x25)) 
	{
		m_camera->MoveUp(m_timer->GetTime());
	}

	if (m_input->IsArrowKeyDown(VKey_DownArrow - 0x25)) 
	{
		m_camera->MoveDown(m_timer->GetTime());
	}

	if (m_input->IsRightMouseButtonDown()) 
	{
		if (m_rightClickState == 0)
		{
			XMFLOAT3 mouse3D = m_input->Get3DMouseCoor(m_projectionMatrix, m_viewMatrix, m_worldMatrix);

			m_cube_2->MoveTo(mouse3D.x, mouse3D.z);
			
			m_rightClickState = 1;
		}
	} 
	else 
	{
		m_rightClickState = 0;
	}
		 
	m_cube->Spin(m_timer->GetTime());
	//m_cube_2->Rotate(m_timer->GetTime());

	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}

bool Application::RenderGraphics()
{
	bool result;
	XMFLOAT4X4 worldMatrix;
	
	result = m_dxBase->InitScene();
	if (!result)
	{
		return false;
	}
	
	result = m_camera->Render();
	if (!result)
	{
		return false;
	}

	//Terrain
	worldMatrix = m_terrain->GetWorldMatrix();

	m_terrain->Render(m_dxBase->GetDeviceContext());
	result = m_textureShader->Render(m_dxBase->GetDeviceContext(), worldMatrix, m_viewMatrix, m_projectionMatrix, m_terrain->GetIndexCount(), m_terrain->GetTexture());
	if (!result)
	{
		return false;
	}

	//First Cube
	worldMatrix = m_cube->GetWorldMatrix();

	m_cube->Render(m_dxBase->GetDeviceContext());
	result = m_textureShader->Render(m_dxBase->GetDeviceContext(), worldMatrix, m_viewMatrix, m_projectionMatrix, m_cube->GetIndexCount(), m_cube->GetTexture());
	if (!result)
	{
		return false;
	}

	//Second Cube
	worldMatrix = m_cube_2->GetWorldMatrix();

	m_cube_2->Render(m_dxBase->GetDeviceContext());
	result = m_textureShader->Render(m_dxBase->GetDeviceContext(), worldMatrix, m_viewMatrix, m_projectionMatrix, m_cube_2->GetIndexCount(), m_cube_2->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_dxBase->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_dxBase->TurnOnAlphaBlending();

	//Text
	worldMatrix = m_text->GetWorldMatrix();

	m_text->Render(m_dxBase->GetDeviceContext(), "HELLO");
	result = m_fontShader->Render(m_dxBase->GetDeviceContext(), m_text->GetVertexCount(), m_text->GetWorldMatrix(), m_viewMatrix, m_orthographicMatrix, m_text->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_dxBase->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_dxBase->TurnZBufferOn();
	
	m_dxBase->Present();

	return true;
}