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
	m_AnimTime = 0.0f;
	m_Finished = false;
}


void Animation::Update(float deltaTime)
{
	
	if (!m_Loop)
	{
		if (m_FrameCount != 0 && m_FrameCounter > 0)
		{
			m_AnimTime += deltaTime;
			if (m_AnimTime >= (1.0f / 0.15f))
			{
				m_SpriteFrame++;
				m_AnimTime = 0.0f;
				if (m_SpriteFrame == m_FrameCount - 1)
				{
					m_SpriteFrame = m_EndFrame;
					m_FrameCounter = 0;
				}

			}
		}

		if (m_FrameCounter == 1)
		{
			m_SpriteFrame = m_EndFrame;
		}
	}

	if (m_Loop)
	{
		m_AnimTime += deltaTime;
		if (m_AnimTime >= (1.0f / 0.15f))
		{
			m_SpriteFrame = (m_SpriteFrame + 1) % m_FrameCount;
			m_AnimTime = 0.0f;
		}

		if (m_FrameCount == 1)
		{
			m_SpriteFrame = m_StartingFrame;
		}
	}

}

void Animation::Draw(float x, float y, int spriteWidth, int spriteHeight)
{
	if (m_FrameCount == 0 || m_FrameCount == 1)
	{
		Renderer::GetInstance()->Draw(m_TextureID, x, y, spriteWidth, spriteHeight, 1.0f,1.0f, 1.0f, m_Flip);
	}
	else
	{
		Renderer::GetInstance()->DrawFrame(m_TextureID, x, y, spriteWidth, spriteHeight, m_SpriteRow, m_SpriteFrame, m_StartingFrame, m_FrameCount, m_Flip);
	}
}