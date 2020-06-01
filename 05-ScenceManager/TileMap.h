#pragma once
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
using namespace std;

class CTileMap {
public:
	CTextures *textures;
	CSprites *sprites;
	CGame *game;
	vector<int> map;
	int tileWidth;
	int tileHeight;
	int mapLength;
public:
	CTileMap(LPWSTR texPath, int tileWidth, int tileHeight, int numOfTileCol, int numOfTileRow);
	~CTileMap();
	void initMap(string filePath, int mapLength);
	void renderMap();
};

