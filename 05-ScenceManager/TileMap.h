#pragma once
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"
using namespace std;
class CTileMap
{
	CTextures *textures;
	CSprites *sprites;
	int tileWidth;
	int tileHeight;
	int mapLength;
	CGame *game;
	vector<int> map;
public:
	CTileMap(LPWSTR texPath, int tileWith, int tileHeight,int numOfTileCol,int numOfTileRow);
	CSprite *GetTile(int spriteID) { return sprites->Get(spriteID);};
	void InitMap(string filePath, int mapLength);
	// void SetCam(CGame *game) { this->game = CGame::GetInstance(); this->game = game; };
	void MapLvlRender();
	~CTileMap();
};

