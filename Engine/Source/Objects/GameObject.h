#pragma once
#include "IObject.h"
#include "sdl.h"
#include "box2d.h"
#include "../Physics/Transform.h"
#include "../Physics/Point.h"


/*
*    IObject
*    |--GameObject
*   
*
*    GameObjects Handles Basic Textures without animations
*	 GameObjects Handles basic Translations and Transformations
*    
*/

struct Properties
{

public:
	Properties(std::string textureID, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		X = x;
		Y = y;
		Flip = flip;
		Width = width;
		Height = height;
		TextureID = textureID;
	}

public:
	std::string TextureID;
	int Width, Height;
	float X, Y;
	SDL_RendererFlip Flip;

};

class GameObject : public IObject
{
public:
	GameObject(Properties* properties) :
		m_TextureID(properties->TextureID), 
		m_Width(properties->Width),
		m_Height(properties->Height),
		m_Flip(properties->Flip)
	{
		m_Transform = new Transform(properties->X, properties->Y);
		
		float px = properties->X + properties->Width / 2;
		float py = properties->Y + properties->Height / 2;

		m_Origin = new Point(px, py);
	};
	
	virtual void Init() {  };
	virtual void Draw() = 0;
	virtual void Update(float deltaTime) {  };
	virtual void Clean() = 0;


	inline Point* GetOrigin() { return m_Origin; }

	~GameObject() 
	{
		delete m_Origin;
		delete m_Transform;
	};

protected:
	Point* m_Origin;
	Transform* m_Transform;
	int m_Width;
	int m_Height;
	std::string m_TextureID;
	SDL_RendererFlip m_Flip;

public:
	b2Body* m_Body;
	b2Body* GetBody() { return m_Body; };
	bool m_PendingKill;
	
public:
	bool IsPendingKill() { return m_PendingKill; }
	inline void SetSize(float x, float y) { m_Width = x; m_Height = y; };
};