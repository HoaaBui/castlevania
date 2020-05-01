#include "TileMap.h"
#include <string.h>
#include <sstream>
#include <fstream>
#include "Utils.h"
#include <math.h>
#include <iostream> 
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
	string line;
	char a;
	inputFile.open(fileName);

	if (!inputFile)
		DebugOut(L"file open fail");
	
	// stringstream ss;
	// int number = -1;
	// char test1 = '1';
	// char test2 = '2';
	// char test3 = '3';

	// string str = "";
	// str.push_back(test1);
	// str.push_back(test2);
	// str.push_back(test3);
	// str.push_back(test1);
	// str.push_back(test2);
	// str.push_back(test3);
	// // strncat(str, &test3, 1); 


	// stringstream geek(str);
	// geek >> number; 

	// DebugOut(L"[INFO] This is your happy number %d \n", number);
	/*str = "";*/

	int number = -1;
	string str = "";

	if(inputFile.is_open()){
		// inputFile >> strs;
		// splitString(strs, map, delim);

		getline(inputFile, line);
		
		for(int i=0; line[i] != '\0'; i++){
			a = line[i];
			if(a==','){
				// map.push_back(number);
				stringstream geek(str); // convert string to number
				geek >> number; 
				
				map.push_back(number);
				DebugOut(L"[INFO] This is your happy number %d \n", number);
				
				// i += 1;
				number = -1;
				str = "";
			}else{
				str.push_back(a);
				// DebugOut(L"[INFO] This is your happy number %d \n", number);
			}
		}

		inputFile.close();
	}
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