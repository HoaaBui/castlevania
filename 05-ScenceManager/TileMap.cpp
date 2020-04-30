#include "TileMap.h"
#include <string.h>
#include <sstream>
#include <fstream>
#include "Utils.h"
#include <math.h>
using namespace std;

CTileMap::~CTileMap(){}

CTileMap::CTileMap(LPWSTR texPath, int tWith, int tHeight, int numOfTileCol, int numOfTileRow){
	this->tileWidth = tWith;
	this->tileHeight = tHeight;
	sprites = new CSprites();
	textures = new CTextures();
	int spriteID = 0;
	textures->Add(1,texPath, D3DCOLOR_XRGB(255, 0, 255));

	for (int i = 0; i < numOfTileCol; i++){
		for (int k = 0; k < numOfTileRow; k++){
			sprites->Add(spriteID++, i * tileWidth, tileHeight * k, 
						tileWidth * (i + 1), tileHeight * (k + 1), textures->Get(1));
		}
	}
}

void splitString(const string& str, vector<int> &cont, char delim = ' '){
	stringstream ss(str);
	string token;
	
	while (getline(ss, token, delim)) {
		cont.push_back(stoi(token));
	}
}

void ReadArrayFromTxt(string fileName, vector<int> &map, char delim = ' '){
	ifstream inputFile;
	string strs;
	inputFile.open(fileName);

	if (!inputFile)
		DebugOut(L"file open fail");
	
	while (!inputFile.eof()){
		inputFile >> strs;
		splitString(strs, map, delim);
	}
	inputFile.close();
}

void CTileMap::InitMap(string filePath, int mapLength){
	ReadArrayFromTxt(filePath, this->map, ',');
	this->mapLength = mapLength;
}

void CTileMap::MapLvlRender(){  // dua duong dan vao day
	int startCol = (int(game->GetInstance()->cam_x / tileWidth));					// IMPROVE FOR SCROLLING LVL MAP
	int endCol = startCol + int(game->GetInstance()->screen_width / tileWidth) + 1; // +1 de load tilemap muot hon
	int numOfCol = mapLength / tileWidth;
	int startRow = int(game->GetInstance()->cam_y / tileHeight);
	int endRow = startRow + (map.size() / numOfCol);			//vi Map khong co do sau Y
	float offsetX = floor(-(game->GetInstance()->cam_x)) + startCol * tileHeight; // dung ham floor de tranh hien tuong man hinh bi soc
	float offsetY = floor(-game->GetInstance()->cam_y) + startRow * tileHeight;

	for (int c = startCol; c <= endCol; c++){
		for (int r = startRow; r < endRow; r++){
			int i = numOfCol * r + c;
			if (i <= map.size() - 1){

				CSprite *tile = sprites->Get(map[i]);
				float x = (c - startCol) * tileWidth + offsetX;
				float y = (r - startRow) * tileHeight + offsetY;

				if (tile != NULL){
					tile->Draw(x + game->GetInstance()->cam_x, y + game->GetInstance()->cam_y);
				}
			}
		}
	}
}