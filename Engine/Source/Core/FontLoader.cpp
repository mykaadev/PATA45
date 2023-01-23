#include "FontLoader.h"
#include "glm.hpp"
#include "MathHelper.h"
#include <iostream>


FontLoader* FontLoader::m_Instance = nullptr;


void FontLoader::SetFont(std::string fontDir)
{
	m_FontPath = fontDir;
}


glm::vec2 FontLoader::GetPositionIndexForChar(char c)
{
	int y = 0;
	int x = 0;
	for (int i = 0; i < m_Format.size(); ++i) 
	{
		if (c == m_Format[i]) {
			return glm::vec2(x, y);
		}
		x++;
		if (x >= m_columns) {
			x = 0;
			y++;
		}
	}
	return glm::vec2(0.0f, 0.0f);
}


FontLoader::~FontLoader()
{
	
}

