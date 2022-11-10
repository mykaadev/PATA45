#include "TextureManager.h"
#include "../Core/Engine.h"
#include "../Components/Camera.h"
#include "../Core/CodingHelper.h"
#include "tinyxml.h"
#include <string>

TextureManager* TextureManager::m_Instance = nullptr;

//Add here Option to draw backgrounds
// 
// Add Option to check if the camera is following the player or not
// 
// Add Option to check if we imported an PNG or BMP and behave differently in that case
// 
//this can be used to create the feeling of the parallax:
//Vector2 _cameraPosition = Camera::GetInstance()->GetPosition() * 0.5f; 

bool TextureManager::Load(std::string inID, std::string inFileName)
{
	SDL_Surface* surface = SDL_LoadBMP(inFileName.c_str());
	if (surface == nullptr)
	{
		SDL_Log("Failed to load .BMP texture: %s, %s", inFileName.c_str(), SDL_GetError());
		return false;
	}
	
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
	if (texture == nullptr)
	{
		SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
		return false;
	}


	m_TextureMap[inID] = texture;

	return true;
}

bool TextureManager::ParseTextures(std::string source)
{
	TiXmlDocument _xml;
	_xml.LoadFile(source);

	if (_xml.Error())
	{
		CodingHelper::GetInstance()->DisplayInfo("Failed to load: " + source);
		return false;
	}

	TiXmlElement* _root = _xml.RootElement();
	for (TiXmlElement* e=_root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("texture"))
		{
			std::string _id = e->Attribute("id");
			std::string _src = e->Attribute("source");
			TextureManager::Load(_id, _src);
		}
	}

	CodingHelper::GetInstance()->DisplayInfo("TextureParser loaded:  " + source);
	return true;
}


void TextureManager::Draw(std::string inID, int x, int y, int width, int height, float xScale /*= 1.0f*/, float yScale /*= 1.0f*/, float lagRatio /*= 0.0f*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	SDL_Rect srcRect = { 0, 0, width, height};

	Vector2 _cameraPosition = Camera::GetInstance()->GetPosition() * lagRatio;

	SDL_Rect destRect = { x - _cameraPosition.X, y - _cameraPosition.Y, width * xScale, height * yScale };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);
}


void TextureManager::DrawFrame(std::string inID, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{

	SDL_Rect srcRect = { width*frame, height*(row-1), width, height};

	Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

	SDL_Rect destRect = { x - _cameraPosition.X, y - _cameraPosition.Y, width, height };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, 0, flip);
}



void TextureManager::DrawTile(std::string inTilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	SDL_Rect srcRect = { tileSize * frame, tileSize * row, tileSize, tileSize };

	Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

	SDL_Rect destRect = { x - _cameraPosition.X, y - _cameraPosition.Y, tileSize, tileSize };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inTilesetID], &srcRect, &destRect, 0, nullptr, flip);

}


void TextureManager::Drop(std::string inID)
{
	SDL_DestroyTexture(m_TextureMap[inID]);
	m_TextureMap.erase(inID);
}


void TextureManager::Clean()
{
	std::map<std::string, SDL_Texture*>::iterator i;
	
	for (i = m_TextureMap.begin(); i != m_TextureMap.end(); ++i)
	{
		SDL_DestroyTexture(i->second);
	}

	m_TextureMap.clear();
}


