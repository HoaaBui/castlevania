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
	CTileMap(wstring texPath, int tileWidth, int tileHeight, int numOfTileCol, int numOfTileRow, int textureID, int R, int G, int B);
	~CTileMap();
	void initMap(LPCWSTR filePath, int mapLength);
	void renderMap();
};

