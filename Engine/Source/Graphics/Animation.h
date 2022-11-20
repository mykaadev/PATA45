#pragma once

#include "sdl.h"
#include <string>

class Animation
{
public:
	Animation();
	~Animation();

	void Update(float deltaTime);
	void Draw(float x, float y, int spriteWidth, int spriteHeight);
	void SetProperties(std::string textureID, int spriteRow, int startingFrame, int frameCount, int animSpeed, bool loop = true, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:

	int m_SpriteRow;
	int m_SpriteFrame;
	int m_StartingFrame;
	int m_EndFrame;
	int m_AnimSpeed;
	int m_FrameCount;
	bool m_Loop;
	int m_FrameCounter;
	

	std::string m_TextureID;

	SDL_RendererFlip m_Flip;
};

