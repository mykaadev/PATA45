#pragma once

#include <map>
#include <string>
#include "Level.h"
#include "../Graphics/TileLayer.h"
#include "tinyxml.h"

class LevelParser
{
public:
	inline static LevelParser* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new LevelParser(); }

	bool Load();
	void Clean();
	
	inline Level* GetLevel(std::string id) { return m_LevelDictionary[id];}

private:
	bool Parse(std::string id, std::string source);
	Tileset ParseTileset(TiXmlElement* xmlTileset);
	TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TilesetList inTilesetList, int inTileSize, int inRowCount, int inColCount);

private:
	LevelParser() {};

	static LevelParser* m_Instance;
	std::map<std::string, Level*> m_LevelDictionary;
};

