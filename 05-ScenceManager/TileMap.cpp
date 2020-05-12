#include "TileMap.h"
#include <fstream>
#include <string.h>
#include <sstream>
#include "Utils.h"
#include <math.h>
#include <iostream> 
using namespace std;

CTileMap::~CTileMap(){}

CTileMap::CTileMap(LPWSTR texPath, int tWidth, int tHeight, int numOfTileCol, int numOfTileRow){
	this->tileWidth = tWidth;
	this->tileHeight = tHeight;
	sprites = new CSprites();
	textures = new CTextures();
	textures->Add(1,texPath, D3DCOLOR_XRGB(255, 0, 255));

	int spriteID = 0;

	for (int i = 0; i < numOfTileCol; i++){
		for (int k = 0; k < numOfTileRow; k++){
			sprites->Add(spriteID++, i * tileWidth, tileHeight * k, 
						tileWidth * (i + 1), tileHeight * (k + 1), textures->Get(1));
		}
	}
}

void CTileMap::initMap(string filePath, int mapLength){
	ifstream inputFile;
	string line;
	char a;
	int number = -1;
	string str = "";

	inputFile.open(filePath);
	if (!inputFile) DebugOut(L"file open fail");

	if(inputFile.is_open()){
		// lay tung dong trong file text
		getline(inputFile, line);
		
		for(int i=0; line[i] != '\0'; i++){
			a = line[i];

			if(a == ','){
				stringstream geek(str); // convert string to number
				geek >> number; 
				
				this->map.push_back(number);
				// DebugOut(L"[INFO] This is your happy number %d \n", number);
				
				number = -1;
				str = "";
			}else{
				str.push_back(a);
				// DebugOut(L"[INFO] This is your happy number %d \n", number);
			}
		}

		inputFile.close();
	}

	this->mapLength = mapLength;
}

void CTileMap::renderMap(){
	int startCol = (int(game->GetInstance()->cam_x / tileWidth));
	int endCol = startCol + int(game->GetInstance()->screen_width / tileWidth) + 1; // +1 de load tilemap muot hon
	int numOfCol = mapLength / tileWidth;
	int startRow = int(game->GetInstance()->cam_y / tileHeight);
	int endRow = startRow + (map.size() / numOfCol);			//vi map khong co do sau Y
	// gia tri offset giup cho map load muot hon
	float offsetX = floor(-(game->GetInstance()->cam_x)) + startCol * tileHeight; // dung ham floor de tranh hien tuong man hinh bi soc
	float offsetY = floor(-game->GetInstance()->cam_y) + startRow * tileHeight;

	for (int c = startCol; c <= endCol; c++){
		for (int r = startRow; r < endRow; r++){
			//Do map duoc danh so theo kieu tung hang tu trai qua phai, tu tren xuong duoi 
			// nen phai xac dinh index de goi cai tile
			int i = numOfCol * r + c;
			if (i <= map.size() - 1){

				CSprite *tile = sprites->Get(map[i]);
				float x = (c - startCol) * tileWidth + offsetX;
				float y;
				if(CGame::GetInstance()->current_scene == 1){
					y = (r - startRow) * tileHeight + offsetY;//+32
				}else{
					y = (r - startRow) * tileHeight + offsetY + 32;
				}

				if (tile != NULL){
					tile->Draw(x + game->GetInstance()->cam_x, y + game->GetInstance()->cam_y);
				}
			}
		}
	}
}