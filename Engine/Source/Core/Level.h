#pragma once

#include "../Graphics/Layer.h"
#include <vector>

class Level
{
public:
	void Render();
	void Update();
	std::vector<Layer*> GetLevelLayers() { return m_LevelLayers; }


private:
	friend class LevelParser;
	std::vector<Layer*> m_LevelLayers;
};

