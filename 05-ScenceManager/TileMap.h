#pragma once
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
using namespace std;

class CTileMap{
public:
	CTextures *textures;
	CSprites *sprites;
	int tileWidth;
	int tileHeight;
	int mapLength;
	CGame *game;
	vector<int> map;
public:
	CTileMap(LPWSTR texPath, int tileWith, int tileHeight, int numOfTileCol, int numOfTileRow);
	~CTileMap();
	void initMap(string filePath, int mapLength);
	void renderMap();
};

