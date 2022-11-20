#include "Camera.h"
#include "../Core/Engine.h"



Camera* Camera::m_Instance = nullptr;


Camera::Camera()
{
	m_Viewport = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SetCameraFollowTarget(true);
	SetCameraTarget(nullptr);

	m_Target = nullptr;
	m_Instance = nullptr;
}


Camera::~Camera()
{
	delete m_Instance;
	delete m_Target;
}

void Camera::Update(float deltaTime)
{
	if (bCameraFollowsTarget && m_Target != nullptr)
	{
		m_Viewport.x = m_Target->X - SCREEN_WIDTH / 2;
		m_Viewport.y = m_Target->Y - SCREEN_HEIGHT / 2;

		m_Position = Vector2(m_Viewport.x, m_Viewport.y);
	}
}

