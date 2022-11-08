#include "Camera.h"
#include "../Core/Engine.h"


Camera* Camera::m_Instance = nullptr;


Camera::Camera()
{
	m_Viewport = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SetCameraFollowTarget(true);
	SetCameraTarget(nullptr);

}


void Camera::Update(float deltaTime)
{
	if (bCameraFollowsTarget && m_Target != nullptr)
	{
		m_Viewport.x = m_Target->X - SCREEN_WIDTH / 2;
		m_Viewport.y = m_Target->Y - SCREEN_HEIGHT / 2;

		if (m_Viewport.x < 0) { m_Viewport.x = 0; }
		if (m_Viewport.y < 0) { m_Viewport.y = 0; }

		if (m_Viewport.x > (2 * SCREEN_WIDTH - m_Viewport.w)) { m_Viewport.x = (SCREEN_WIDTH - m_Viewport.w); }
		if (m_Viewport.y > (2 * SCREEN_HEIGHT - m_Viewport.h)) { m_Viewport.y = (SCREEN_HEIGHT - m_Viewport.h); }

		m_Position = Vector2(m_Viewport.x, m_Viewport.y);

	}
}

