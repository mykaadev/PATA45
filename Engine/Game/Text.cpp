#include "Text.h"
#include "World.h"
#include "Renderer.h"
#include "FontLoader.h"
#include "glm.hpp"

Text::Text(Properties* props) : Character(props)
{
	m_Text = "TEST";
}


void Text::Init()
{
	
}

void Text::SetText(std::string newText)
{
	if (newText == m_Text) {
		return;
	}
	m_Text = newText;
	m_textCoords.clear();
	for (char& c : m_Text) {
		//Find sprite for char
		if (m_Font) {
			glm::vec2 IndexPos = FontLoader::GetInstance()->GetPositionIndexForChar(c);
			m_textCoords.push_back(IndexPos);
		}
	}
}


void Text::Draw()
{
	glm::vec2 posWorld = glm::vec2(m_Origin->X, m_Origin->Y);

	for (int i = 0; i < m_textCoords.size(); ++i) {

		SDL_Rect SourceRect;

		int m_FrameWidth = 128 / 8;
		int m_FrameHeight = 192 / 12;

		SourceRect.x = m_FrameWidth * m_textCoords[i].x;
		SourceRect.y = m_FrameHeight * m_textCoords[i].y;


		SourceRect.w = m_FrameWidth;
		SourceRect.h = m_FrameHeight;

	
		Renderer::GetInstance()->Draw("fontpng", m_Origin->X, m_Origin->Y, 100, 100);
		posWorld = glm::vec2(m_Origin->X + m_FrameWidth, m_Origin->Y);
	}
}

void Text::Update(float deltaTime)
{
	
}


void Text::SetOriginPoint()
{
	
}
void Text::Clean()
{

}

void Text::CheckCollision(GameObject* otherGameObject)
{

}



void Text::SetupBody()
{
	b2BodyDef _BodyDef;
	_BodyDef.type = b2_dynamicBody;
	_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
	_BodyDef.gravityScale = 0.0f;
	_BodyDef.fixedRotation = true;
	_BodyDef.bullet = true;

	m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

	b2PolygonShape _boxShape;
	_boxShape.SetAsBox(0.1f, 0.1f );

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_boxShape;
	_fixtureDef.isSensor = true;



	b2Fixture* _Fixture;
	_Fixture = m_Body->CreateFixture(&_fixtureDef);
}