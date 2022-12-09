#include "TileLayer.h"
#include "Renderer.h"
TileLayer::TileLayer(int tileSize, int rowCount, int colCount, TileMap tileMap, TilesetList tilesetList) : 
	
	m_TileSize (tileSize),  
	m_RowCount(rowCount),
	m_ColCount (colCount), 
	m_Tilemap (tileMap), 
	m_TilesetList(tilesetList)
{
	
	for (unsigned int i = 0; i < m_TilesetList.size(); ++i)
	{
		Renderer::GetInstance()->Load(m_TilesetList[i].tileName, "../Assets/Game/Levels/" + m_TilesetList[i].tileSource);

	}

}

void TileLayer::Render()
{
	for (unsigned int i = 0; i < m_RowCount; ++i)
	{
		for (unsigned int j = 0; j < m_ColCount; ++j)
		{
			int _tileID = m_Tilemap[i][j];

			if (_tileID == 0)
			{
				continue;
			}
			else
			{
				int _tileIndex = 0;

				if (m_TilesetList.size() > 1)
				{
					for (unsigned int k = 1; k < m_TilesetList.size(); ++k)
					{
						if (_tileID >= m_TilesetList[k].firstID && _tileID <= m_TilesetList[k].lastID)
						{
							_tileID = _tileID + m_TilesetList[k].tileCount - m_TilesetList[k].lastID;
							_tileIndex = k;
							break;
						}
					}
				}

				Tileset _tileset = m_TilesetList[_tileIndex];
				int _tileRow = _tileID / _tileset.colCount;
				int _tileCol = _tileID - _tileRow * _tileset.colCount - 1;

				if (_tileID % _tileset.colCount == 0)
				{
					--_tileRow;
					_tileCol = _tileset.colCount - 1;
				}

				Renderer::GetInstance()->DrawTile(_tileset.tileName, _tileset.tileSize,
							j * _tileset.tileSize, i * _tileset.tileSize, _tileRow, _tileCol);


			}
		}
	}
}

void TileLayer::Update()
{

}
