#pragma once
#include <string>
#include <vector>
#include "glm.hpp"


class FontLoader
{
public:


	static FontLoader* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new FontLoader(); }
	static FontLoader* m_Instance;

	void SetFont(std::string font);
	std::string m_FontPath;


	int m_rows = 12;
	int m_columns = 8;
	std::vector<char> m_Format = { ' ','!','"','#','$','%','&',39,
								'(',')','*','+',',','-','.','/',
								'0','1','2','3','4','5','6','7',
								'8','9',':',';','<','=','>','?',
								'€','A','B','C','D','E','F','G',
								'H','I','J','K','L','M','N','O',
								'P','Q','R','S','T','U','V','W',
								'X','Y','Z','[',92,']','^','_',
								'`','a','b','c','d','e','f','g',
								'h','i','j','k','l','m','n','o',
								'p','q','r','s','t','u','v','w',
								'x','y','z','{',':','}','»','«' };
public:
	
	std::string GetFontPath() { return m_FontPath; }

	//Get the Sprite location
	glm::vec2 GetPositionIndexForChar(char c);

	int GetRows() { return m_rows; }
	int GetColumns() { return m_columns; }

	void SetFormat(std::vector<char> newFormat) { m_Format = newFormat; }
	void SetRowsAndColumns(int rows, int columns) { m_rows = rows; m_columns = columns; }

	~FontLoader();

};

