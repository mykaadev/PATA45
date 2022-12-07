#pragma once
#include "../Source/Objects/Character.h"
#include "box2d.h"

class BackgroundProps : public Character
{
public:
	BackgroundProps(Properties* props);

	void Init() override;
	void Draw() override;
	void Update(float deltaTime) override;
	void Clean() override;
	void CheckCollision(GameObject* otherGameObject) override;
	void SetAnimationProps(std::string FileName, bool isFlippedHorizontaly);

	inline Animation* GetAnimation() { return m_Animation; }
	b2Vec2 m_DefaultPos;
	b2Body* m_Body;

	void SetupBody();
protected:
	Animation* m_Animation;


private:
	void SetOriginPoint();
};

