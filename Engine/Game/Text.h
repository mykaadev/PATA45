#pragma once
#include "../Source/Objects/Character.h"
#include "box2d.h"
#include "glm.hpp"
#include <vector>

class Text : public Character
{
public:
	Text(Properties* props);

	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;

	std::string m_Text;
	float m_TextScale = 1;
	glm::vec2 m_TextPosition;
	class FontText* m_Font;
	std::vector<glm::vec2> m_textCoords;


	void SetText(std::string newText);
	inline void SetScale(float textScale) { m_TextScale = textScale; }
	inline void SetPosition(glm::vec2 newPosition) { m_TextPosition = newPosition; }

	inline std::string GetTextString() { return m_Text; }
	inline float GetTextScale() { return m_TextScale; }
	inline glm::vec2 GetTextPosition() { return m_TextPosition; }

	b2Body* m_Body;

	void SetupBody();

protected:
	Animation* m_Animation;


private:
	void SetOriginPoint();
};

