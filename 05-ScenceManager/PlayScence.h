#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "BrickScene2.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "TileMap.h"
#include "Whip.h"
#include "Light.h"
#include "Knife.h"
#define MAP1_LENGTH			1472
#define MAP2_LENGTH			512
#define MAP3_LENGTH			1024
#define MAP4_LENGTH			1536

class CPlayScene: public CScene
{
public: 
	CMario *player;					// A play scene has to have player, right?
	CMario *mario;		 
	CTileMap *map1;
	CTileMap *map2;
	CTileMap *map3;
	CTileMap *map4;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void initPlayer() { mario = CMario::GetInstance(); };

	friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

