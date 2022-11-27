#include "Level.h"

void Level::Render()
{
	for (unsigned int i = 0; i < m_LevelLayers.size(); ++i)
	{
		m_LevelLayers[i]->Render();
	}
}

void Level::Update()
{
	for (unsigned int i = 0; i < m_LevelLayers.size(); ++i)
	{
		m_LevelLayers[i]->Update();
	}
}
