#pragma once
#include <string>
#include "SDL.h"
#include <map>

class TextureManager
{
public:
	static TextureManager* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new TextureManager(); }

	bool Load(std::string inID, std::string inFileName);
	bool ParseTextures(std::string source);
	void Drop(std::string inID);
	void Clean();

	void Draw(std::string inID, int x, int y, int width, int height, float xScale = 1.0f, float yScale = 1.0f, float lagRatio = 0.5f, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawTile(std::string inTilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawFrame(std::string inID, int x, int y, int width, int height, int row, int currentFrame, int startingFrame, int frameCount, SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	TextureManager() {};
	static TextureManager* m_Instance;

	std::map<std::string, SDL_Texture*> m_TextureMap;

};

