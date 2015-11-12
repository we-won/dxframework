#include "Application.h"


Application::Application()
: m_dxBase(0), m_colorShader(0), m_textureShader(0), m_fontShader(0), m_camera(0), m_timer(0), m_input(0), m_light(0), m_renderTexture(0), m_depthShader(0),
	m_rightClickState(0)
{
	m_CubeModel = 0;
	m_GroundModel = 0;
	m_SphereModel = 0;
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
		MessageBox(0, "Error initializing DXBase!", "Compile Error", MB_OK);
		return false;
	}

	m_colorShader = new ColorShader;
	if (!m_colorShader)
	{
		return false;
	}

	if (!m_colorShader->Initialize(m_dxBase->GetDevice()))
	{
		MessageBox(0, "Error initializing Color Shader!", "Compile Error", MB_OK);
		return false;
	}

	m_textureShader = new TextureShader;
	if (!m_textureShader)
	{
		return false;
	}

	if (!m_textureShader->Initialize(m_dxBase->GetDevice()))
	{
		MessageBox(0, "Error initializing Texture Shader!", "Compile Error", MB_OK);
		return false;
	}

	m_fontShader = new FontShader;
	if (!m_fontShader)
	{
		return false;
	}

	if (!m_fontShader->Initialize(m_dxBase->GetDevice()))
	{
		MessageBox(0, "Error initializing Font Shader!", "Compile Error", MB_OK);
		return false;
	}

	m_camera = new Camera;
	if (!m_camera)
	{
		return false;
	}

	if (!m_camera->Render())
	{
		MessageBox(0, "Error initializing Camera!", "Compile Error", MB_OK);
		return false;
	}

	m_timer = new TimerLx;
	if (!m_timer)
	{
		return false;
	}

	if (!m_timer->Initialize())
	{
		MessageBox(0, "Error initializing Timer!", "Compile Error", MB_OK);
		return false;
	}

	m_input = new InputLx;
	if (!m_input)
	{
		return false;
	}

	if (!m_input->Initialize(hwnd, width, height))
	{
		MessageBox(0, "Error initializing Input!", "Compile Error", MB_OK);
		return false;
	}

	m_light = new LightLx;
	if (!m_light)
	{
		return false;
	}

	if (!m_light->Initialize(m_dxBase->GetDevice()))
	{
		MessageBox(0, "Error initializing Light!", "Compile Error", MB_OK);
		return false;
	}

	m_renderTexture = new RenderTexture;
	if (!m_renderTexture)
	{
		return false;
	}

	if (!m_renderTexture->Initialize(m_dxBase->GetDevice(), 1024, 1024, 1.0f, 100.0f))
	{
		MessageBox(0, "Error initializing Render Texture!", "Compile Error", MB_OK);
		return false;
	}

	m_depthShader = new DepthShader;
	if (!m_depthShader)
	{
		return false;
	}

	if (!m_depthShader->Initialize(m_dxBase->GetDevice()))
	{
		MessageBox(0, "Error initializing Depth Shader!", "Compile Error", MB_OK);
		return false;
	}

	// --- DISPLAYS ---
	bool result;
	
	// Create the cube model object.
	m_CubeModel = new ModelLx;
	if(!m_CubeModel)
	{
		return false;
	}

	// Initialize the cube model object.
	result = m_CubeModel->Initialize(m_dxBase->GetDevice(), "../LubiEngine/data/cube.txt", "wall01.dds");
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the cube model object.", "Error", MB_OK);
		return false;
	}

	m_CubeModel->SetPosition(-2.0f, 2.0f, 0.0f);

	// Create the cube model object.
	m_SphereModel = new ModelLx;
	if(!m_SphereModel)
	{
		return false;
	}

	// Initialize the cube model object.
	result = m_SphereModel->Initialize(m_dxBase->GetDevice(), "../LubiEngine/data/sphere.txt", "ice.dds");
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the sphere model object.", "Error", MB_OK);
		return false;
	}

	m_SphereModel->SetPosition(2.0f, 2.0f, 0.0f);

	// Create the ground model object.
	m_GroundModel = new ModelLx;
	if(!m_GroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = m_GroundModel->Initialize(m_dxBase->GetDevice(), "../LubiEngine/data/plane01.txt", "metal001.dds");
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the ground model object.", "Error", MB_OK);
		return false;
	}

	// Set the position for the ground model.
	m_GroundModel->SetPosition(0.0f, 1.0f, 0.0f);

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

	if (m_light)
	{
		m_light->ReleaseObjects();
		delete m_light;
		m_light = 0;
	}

	if (m_renderTexture)
	{
		m_renderTexture->ReleaseObjects();
		delete m_renderTexture;
		m_renderTexture = 0;
	}

	if (m_depthShader)
	{
		m_depthShader->ReleaseObjects();
		delete m_depthShader;
		m_depthShader = 0;
	}

	// --- DISPLAYS ---

	// Release the ground model object.
	if(m_GroundModel)
	{
		m_GroundModel->ReleaseObjects();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	// Release the sphere model object.
	if(m_SphereModel)
	{
		m_SphereModel->ReleaseObjects();
		delete m_SphereModel;
		m_SphereModel = 0;
	}

	// Release the cube model object.
	if(m_CubeModel)
	{
		m_CubeModel->ReleaseObjects();
		delete m_CubeModel;
		m_CubeModel = 0;
	}
}

void Application::HandleInput(LPARAM lParam)
{
	m_input->GetData(lParam);
}

bool Application::Frame()
{
	bool result;
	
	result = m_camera->Render();
	if (!result)
	{
		return false;
	}

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

			m_CubeModel->SetPosition(mouse3D.x, 2.0f, mouse3D.z);
			
			m_rightClickState = 1;
		}
	} 
	else 
	{
		m_rightClickState = 0;
	}

	result = RenderGraphics();
	if (!result)
	{
		return false;
	}

	return result;
}

bool Application::RenderSceneToTexture()
{
	bool result;
	XMFLOAT4X4 worldMatrix, lightViewMatrix, lightProjectionMatrix;
	XMMATRIX worldMat;
	float posX, posY, posZ;

	lightViewMatrix = m_light->GetViewMatrix();
	lightProjectionMatrix = m_light->GetProjectionMatrix();

	m_renderTexture->SetRenderTarget(m_dxBase->GetDeviceContext());
	m_renderTexture->ClearRenderTarget(m_dxBase->GetDeviceContext());

	// Cube
	m_CubeModel->GetPosition(posX, posY, posZ);
	worldMat = XMMatrixTranslation(posX, posY, posZ);
	XMStoreFloat4x4(&worldMatrix, worldMat);

	m_CubeModel->Render(m_dxBase->GetDeviceContext());
	result = m_depthShader->Render(m_dxBase->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	// Sphere
	m_SphereModel->GetPosition(posX, posY, posZ);
	worldMat = XMMatrixTranslation(posX, posY, posZ);
	XMStoreFloat4x4(&worldMatrix, worldMat);

	m_SphereModel->Render(m_dxBase->GetDeviceContext());
	result = m_depthShader->Render(m_dxBase->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	// Ground
	m_GroundModel->GetPosition(posX, posY, posZ);
	worldMat = XMMatrixTranslation(posX, posY, posZ);
	XMStoreFloat4x4(&worldMatrix, worldMat);

	m_GroundModel->Render(m_dxBase->GetDeviceContext());
	result = m_depthShader->Render(m_dxBase->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	if(!result)
	{
		return false;
	}

	m_dxBase->SetBackBufferRenderTarget();
	m_dxBase->ResetViewport();

	return true;
}

bool Application::RenderGraphics()
{
	bool result;
	XMMATRIX worldMat;
	XMFLOAT4X4 worldMatrix;
	float posX, posY, posZ;

	result = RenderSceneToTexture();
	if (!result)
	{
		return false;
	}

	result = m_dxBase->InitScene();
	if (!result)
	{
		return false;
	}

	// Cube
	m_CubeModel->GetPosition(posX, posY, posZ);
	worldMat = XMMatrixTranslation(posX, posY, posZ);
	XMStoreFloat4x4(&worldMatrix, worldMat);

	m_CubeModel->Render(m_dxBase->GetDeviceContext());
	result = m_textureShader->Render(m_dxBase->GetDeviceContext(), worldMatrix, m_viewMatrix, m_projectionMatrix, m_CubeModel->GetIndexCount(),
										m_CubeModel->GetTexture(),  m_light->GetViewMatrix(), m_light->GetProjectionMatrix(), m_renderTexture->GetShaderResourceView());

	// Sphere
	m_SphereModel->GetPosition(posX, posY, posZ);
	worldMat = XMMatrixTranslation(posX, posY, posZ);
	XMStoreFloat4x4(&worldMatrix, worldMat);

	m_SphereModel->Render(m_dxBase->GetDeviceContext());
	result = m_textureShader->Render(m_dxBase->GetDeviceContext(), worldMatrix, m_viewMatrix, m_projectionMatrix, m_SphereModel->GetIndexCount(),
										m_SphereModel->GetTexture(),  m_light->GetViewMatrix(), m_light->GetProjectionMatrix(), m_renderTexture->GetShaderResourceView());

	// Ground
	m_GroundModel->GetPosition(posX, posY, posZ);
	worldMat = XMMatrixTranslation(posX, posY, posZ);
	XMStoreFloat4x4(&worldMatrix, worldMat);

	m_GroundModel->Render(m_dxBase->GetDeviceContext());
	result = m_textureShader->Render(m_dxBase->GetDeviceContext(), worldMatrix, m_viewMatrix, m_projectionMatrix, m_GroundModel->GetIndexCount(),
										m_GroundModel->GetTexture(),  m_light->GetViewMatrix(), m_light->GetProjectionMatrix(), m_renderTexture->GetShaderResourceView());

	m_dxBase->Present();

	return true;
}