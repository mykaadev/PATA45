#include "Animation.h"
#include "Renderer.h"
#include <iostream>

Animation::Animation()
{

}

Animation::~Animation()
{

}


void Animation::SetProperties(std::string textureID, int spriteRow, int startingFrame, int frameCount, int animSpeed, bool loop /*= true*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	m_TextureID = textureID;
	m_SpriteRow = spriteRow;
	m_FrameCount = frameCount;
	m_StartingFrame = startingFrame;
	m_EndFrame = startingFrame + frameCount-1;
	m_AnimSpeed = animSpeed;
	m_Flip = flip;
	m_Loop = loop;
	m_FrameCounter = m_FrameCount;
}


void Animation::Update(float deltaTime)
{
	if (!m_Loop)
	{
		if (m_FrameCount != 0 && m_FrameCounter > 0)
		{
			if (m_SpriteFrame == m_FrameCount - 1)
			{
				m_SpriteFrame = m_EndFrame;
			}
			else
			{
				m_SpriteFrame = (SDL_GetTicks() / m_AnimSpeed) % m_FrameCount;
			}
		}

		if (m_FrameCounter == 0)
		{
			m_SpriteFrame = m_EndFrame;
		}
	}
	

	if (m_Loop)
	{
		m_SpriteFrame = (SDL_GetTicks() / m_AnimSpeed) % m_FrameCount;		

		if (m_FrameCount == 0)
		{
			m_SpriteFrame = m_StartingFrame;
		}
	}
}

void Animation::Draw(float x, float y, int spriteWidth, int spriteHeight)
{
	Renderer::GetInstance()->DrawFrame(m_TextureID, x, y, spriteWidth, spriteHeight, m_SpriteRow, m_SpriteFrame, m_StartingFrame, m_FrameCount, m_Flip);
}