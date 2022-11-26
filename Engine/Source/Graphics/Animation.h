#pragma once

#include "sdl.h"
#include <string>


class Animation
{
public:
	Animation();
	~Animation();

	//Update the animation current frame
	void Update(float deltaTime);

	//Sends the data to the Texture Manager so it can be drawn on screen
	void Draw(float x, float y, int spriteWidth, int spriteHeight);

	//Sets the properties for this animation 
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

