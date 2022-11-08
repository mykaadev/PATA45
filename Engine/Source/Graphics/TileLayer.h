#pragma once
#include "Layer.h"
#include <vector>
#include <string>


struct Tileset
{
	int firstID = 0;
	int lastID = 0;
	int rowCount = 0;
	int colCount = 0;
	int tileCount = 0;
	int tileSize = 0;
	std::string tileName = "DefName";
	std::string tileSource = "DefSRC";
};

using TilesetList = std::vector<Tileset>;
using TileMap = std::vector<std::vector<int> >;

class TileLayer : public Layer
{
public:
	TileLayer(int tileSize, int rowCount, int colCount, TileMap tileMap, TilesetList tilesetList);

	virtual void Render();
	virtual void Update();

	inline TileMap GetTileMap() { return m_Tilemap; }

private:
	int m_TileSize;
	int m_RowCount;
	int m_ColCount;
	TileMap m_Tilemap;
	TilesetList m_TilesetList;

};

