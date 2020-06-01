#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "TileMap.h"
#include "Whip.h"
#include "Light.h"
#include "Knife.h"
#include "Knight.h"
#include "SmallCandle.h"
#include "Knight.h"
#include "Mario.h"
#include "Boomerang.h"
#include "Heart.h"
#include "BrickScene2.h"
#include "BrickStair.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	// mario = CMario::GetInstance();
	// Khong hieu tai sao ham khoi tao lai chay nhieu lan nen viet ham rieng tao gia tri cho SIMON
	// dung 1 lan.
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_WHIP	4
#define OBJECT_TYPE_LIGHT	5
#define OBJECT_TYPE_KNIFE	6
#define OBJECT_TYPE_BOOMERANG	7
#define OBJECT_TYPE_HEART	8
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_BRICK_SCENE2	9 //Khong dung
#define OBJECT_TYPE_KNIGHT	10
#define OBJECT_TYPE_SMALL_CANDLE	11
#define OBJECT_TYPE_BRICK_STAIR	12
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;
	CGameObject *objWhip = NULL;
	CWhip *whip = new CWhip();

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		// if (mario!=NULL) 
		// {
		// 	DebugOut(L"[ERROR] MARIO object was created before! ");
		// 	return;
		// }
		// obj = new CMario(); 
		// mario = (CMario*)obj;  
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	// case OBJECT_TYPE_BRICK_SCENE2: obj = new CBrickScene2(); break;
	case OBJECT_TYPE_LIGHT: obj = new CLight(); break;
	case OBJECT_TYPE_WHIP:  obj = new CWhip(); break;
	case OBJECT_TYPE_KNIFE:  obj = new CKnife(); break;
	case OBJECT_TYPE_KNIGHT:  obj = new CKnight(); break;
	case OBJECT_TYPE_SMALL_CANDLE:	obj = new CSmallCandle(); break;
	case OBJECT_TYPE_BOOMERANG:  obj = new CBoomerang(); break;
	case OBJECT_TYPE_HEART:  obj = new CHeart(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_BRICK_STAIR:
	{
		// DebugOut(L"[INFO] Co chay vo ham nay abcd\n");
		obj = new CBrickStair(); 
		//float y2 = atof(tokens[4].c_str());
		//DebugOut(L"[INFO] Gia tri ny: %f \n",y2);
		//obj->ny2 = y2;
		break;
	} 
	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	if(object_type == OBJECT_TYPE_MARIO){
		mario->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		mario->SetAnimationSet(ani_set);
	}else if(object_type == OBJECT_TYPE_KNIFE){
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		mario->knife = (CKnife*)obj; 
	}else if(object_type == OBJECT_TYPE_BOOMERANG){
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		mario->boomerang = (CBoomerang*)obj; 
	}else{
		// General object setup
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}
}

void CPlayScene::Load(){
	// khoi tao gia tri cho SIMON
	this->mario = CMario::GetInstance();
	map1 = new CTileMap(L"textures\\map1_tiled.PNG", 64, 64, 14, 8);
	//map1 = new CTileMap(L"textures\\map1_tiled.PNG", 64, 64, 16, 2);
	map1->initMap("map1.txt", MAP1_LENGTH);
	
	map2 = new CTileMap(L"textures\\map2_tiled.PNG", 64, 64, 16, 2);
	map2->initMap("map2.txt", MAP2_LENGTH);
	
	map3 = new CTileMap(L"textures\\map3_tiled.PNG", 64, 64, 10, 5);
	map3->initMap("map3.txt", MAP3_LENGTH);

	map4 = new CTileMap(L"textures\\map4_tiled.PNG", 64, 64, 17, 5);
	map4->initMap("map4.txt", MAP4_LENGTH);

	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++){
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++){
		objects[i]->Update(dt, &coObjects);
	}

	mario->Update(dt,&coObjects);
	
	// Update camera to follow mario
	float cx, cy;
	mario->GetPosition(cx, cy);
	//Update Simon position

	int a = CGame::GetInstance()->current_scene;
	// DebugOut(L"[INFO] This is your current scene: %d\n", a);
	// DebugOut(L"[INFO] Position of Simon X: %f\n", cx);
	// DebugOut(L"[INFO] Position of Simon Y: %f\n", cy);
	if(a==2){
		if(cx<=70){
			mario->SetPosition(70,cy);
		}else if(cx>=407){
			mario->SetPosition(407,cy);
		}
		cx=255;
	}else if(a==1){
		if(cx<=0){
			mario->SetPosition(0,cy);
		}else if(cx>=1300){
			mario->SetPosition(1300,cy);
		}
		//Update camera position
		if( cx<0 || (cx>=0 && cx<=255) ){
			cx=255;
		}else if(cx>=1093){
			cx=1093;
		}
	}else if(a==4){ // scene 2.2
		if(cx<=0){
			mario->SetPosition(0,cy);
		}else if(cx>=920){
			mario->SetPosition(920,cy);
		}
		//Update camera position
		if( cx<0 || (cx>=0 && cx<=255) ){
			cx=255;
		}else if(cx>=770){
			cx=770;
		}
	}else if(a==5){ // scene 3.1
		if(cx<=0){
			mario->SetPosition(0,cy);
		}else if(cx>=1430){
			mario->SetPosition(1430,cy);
		}
		//Update camera position
		if( cx<0 || (cx>=0 && cx<=250) ){
			cx=250;
		}else if(cx>=1285){
			cx=1285;
		}
	}
	// DebugOut(L"[INFO] Position of Simon X: %f\n", cx);
	// DebugOut(L"[INFO] Position of Simon Z: %f\n", cy);

	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx,0);
	
}

void CPlayScene::Render(){
	if(CGame::GetInstance()->current_scene == 1){
		map1->renderMap();
	}else if(CGame::GetInstance()->current_scene == 2){
		map2->renderMap();
	}else if(CGame::GetInstance()->current_scene == 4){
		map3->renderMap();
	}else if(CGame::GetInstance()->current_scene == 5){
		map4->renderMap();
	}
	for (int i = 0; i < objects.size(); i++){
		objects[i]->Render();
	}
	mario->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	mario = NULL;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode){
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	//CMario *mario = ((CPlayScene*)scence)->player;
	CMario *marioo = CMario::GetInstance();
	switch (KeyCode)
	{
	case DIK_SPACE:
		marioo->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A: // reset
		marioo->isUsedWhip = true;
		if (marioo->nx > 0){
			marioo->SetState(MARIO_STATE_ATTACK_STAND_RIGHT);
		}else{
			marioo->SetState(MARIO_STATE_ATTACK_STAND_LEFT);
		}
		break;
	case DIK_S:
		marioo->isUsedSubWeapon = true;
		if (marioo->nx > 0){
			marioo->SetState(MARIO_STATE_ATTACK_STAND_RIGHT);
		}else{
			marioo->SetState(MARIO_STATE_ATTACK_STAND_LEFT);
		}
		break;
	case DIK_D:
		marioo->isUsedSubWeaponBoomerang = true;
		if (marioo->nx > 0){
			marioo->SetState(MARIO_STATE_ATTACK_STAND_RIGHT);
		}else{
			marioo->SetState(MARIO_STATE_ATTACK_STAND_LEFT);
		}
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode){
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	//CMario *mario = ((CPlayScene*)scence)->player;
	CMario *marioo = CMario::GetInstance();
	switch (KeyCode){
		case DIK_DOWN:
			if (marioo->isAttack){
				CWhip * whip = CWhip::GetInstance();
				whip->animation_set->at(0)->SetCurrentFrame(-1);
		        whip->animation_set->at(1)->SetCurrentFrame(-1);
			}
			marioo->isSit = false;
			break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states){
	CGame *game = CGame::GetInstance();
	//CMario *mario = ((CPlayScene*)scence)->player;
	CMario *marioo = CMario::GetInstance();
	// disable control key when Mario die 
	if (marioo->GetState() == MARIO_STATE_DIE) return;

	// Set State for Mario
	if(game->IsKeyDown(DIK_RIGHT)){
		marioo->SetState(MARIO_STATE_WALKING_RIGHT);
	}else if (game->IsKeyDown(DIK_LEFT)){
		marioo->SetState(MARIO_STATE_WALKING_LEFT);
	}else if(game->IsKeyDown(DIK_DOWN) && !marioo->isAttack){
		marioo->SetState(MARIO_STATE_SIT_RIGHT);
		// mario->SetState(MARIO_STATE_SIT_LEFT);
	}else{
		marioo->SetState(MARIO_STATE_IDLE);
	}
}