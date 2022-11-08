#include "LevelParser.h"
#include "tinyxml.h"
#include "../Graphics/TileLayer.h"
#include "Level.h"
#include <iostream>
#include <string>

LevelParser* LevelParser::m_Instance = nullptr;


bool LevelParser::Load()
{
	return Parse("Level0", "../Assets/Game/Levels/Level0.tmx");
}


bool LevelParser::Parse(std::string id, std::string source)
{
	TiXmlDocument _xml;

	_xml.LoadFile(source);

	if (_xml.Error())
	{
		std::cerr << "Failed to load: " << source << std::endl;
		return false;
	}

	TiXmlElement* _root = _xml.RootElement();

	int _rowCount = 0;
	int _colCount = 0;
	int _tileSize = 0;

	_root->AttributeTwo("width", &_colCount);
	_root->AttributeTwo("height", &_rowCount);
	_root->AttributeTwo("tilewidth", &_tileSize);

	TilesetList _tilesetList;
	for (TiXmlElement* e = _root->FirstChildElement(); e!= nullptr; e=e->NextSiblingElement())
	{
		if (e->Value() == std::string("tileset"))
		{
			_tilesetList.push_back(ParseTileset(e));
		}
	}

	Level* _level = new Level();
	for (TiXmlElement* e = _root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("layer"))
		{
			TileLayer* _tileLayer = ParseTileLayer(e, _tilesetList, _tileSize, _rowCount, _colCount);
			_level->m_LevelLayers.push_back(_tileLayer);
		}
	}

	m_LevelDictionary[id] = _level;

	return true;
}

Tileset LevelParser::ParseTileset(TiXmlElement* xmlTileset)
{
	Tileset _tileset;

	_tileset.tileName = *xmlTileset->AttributeOne("name");

	xmlTileset->AttributeTwo("firstgid", &_tileset.firstID);

	xmlTileset->AttributeTwo("tilecount", &_tileset.tileCount);
	_tileset.lastID = (_tileset.firstID + _tileset.tileCount) - 1;

	xmlTileset->AttributeTwo("columns", &_tileset.colCount);
	_tileset.rowCount = _tileset.tileCount / _tileset.colCount;
	xmlTileset->AttributeTwo("tilewidth", &_tileset.tileSize);

 	TiXmlElement* image = xmlTileset->FirstChildElement();
 	_tileset.tileSource = *image->AttributeOne("source");

	return _tileset;

}

TileLayer* LevelParser::ParseTileLayer(TiXmlElement* xmlLayer, TilesetList inTilesetList, int inTileSize, int inRowCount, int inColCount)
{
	TiXmlElement* _data = nullptr;

	for (TiXmlElement* e = xmlLayer->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("data"))
		{
			_data = e;
			break;
		}
	}

	std::string _matrix(_data->GetText());
	std::istringstream _iss(_matrix);
	std::string _id;

	TileMap _tilemap(inRowCount, std::vector<int>(inColCount, 0));

	for (int row = 0; row < inRowCount; row++)
	{
		for (int col = 0; col < inColCount; col++)
		{

			std::getline(_iss, _id,',');
			std::stringstream converter(_id);
			converter >> _tilemap[row][col];

			if (!_iss.good())
			{
				break;
			}
		}
	}

	return (new TileLayer(inTileSize, inRowCount, inColCount, _tilemap, inTilesetList));


}



void LevelParser::Clean()
{
	std::map<std::string, Level*>::iterator i;
	for (i = m_LevelDictionary.begin(); i != m_LevelDictionary.end(); i++)
	{
		i->second = nullptr;
	}

	m_LevelDictionary.clear();
}