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
#include "SmallCandle.h"
#include "Simon.h"
#include "Boomerang.h"
#include "Heart.h"
#include "BrickScene2.h"
#include "BrickStair.h"
#include "WhipIcon.h"
#include "KnifeIcon.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
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

#define SCENE_SECTION_TILEMAP_IMAGE		7
#define SCENE_SECTION_TILEMAP_MAPTXT	8

#define OBJECT_TYPE_SIMON	0
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
#define OBJECT_TYPE_WHIP_ICON	14 // khong dung so 13
#define OBJECT_TYPE_KNIFE_ICON	15
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_TILEMAP_IMAGE(string line){
	vector<string> tokens = split(line);
	if (tokens.size() < 9) {return;} // skip invalid lines
	
	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());
	int tWidth = atoi(tokens[5].c_str());
	int tHeight = atoi(tokens[6].c_str());
	int numOfTileCol = atoi(tokens[7].c_str());
	int numOfTileRow = atoi(tokens[8].c_str());

	map = new CTileMap(path, tWidth, tHeight, numOfTileCol, numOfTileRow, texID, R, G, B);
}

void CPlayScene::_ParseSection_TILEMAP_MAPTXT(string line){
	vector<string> tokens = split(line);
	if (tokens.size() < 4) return; // skip invalid lines

	int mapNumber = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);
	int mapLength = atoi(tokens[2].c_str());
	int texID = atoi(tokens[3].c_str());

	map->initMap(path, mapLength);
}

void CPlayScene::_ParseSection_TEXTURES(string line){
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines
	// DebugOut(L"[INFO] This is your happy commandabc: \n");
	int texID = atoi(tokens[0].c_str());
	// DebugOut(L"[INFO] This is your happy number texID %d \n", texID);
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

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON: break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_WHIP_ICON: obj = new CWhipIcon(); break;
	case OBJECT_TYPE_KNIFE_ICON: obj = new CKnifeIcon(); break;
	case OBJECT_TYPE_LIGHT: obj = new CLight(); break;
	case OBJECT_TYPE_WHIP:  obj = new CWhip(); break;
	case OBJECT_TYPE_KNIFE:  obj = new CKnife(); break;
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

	if(object_type == OBJECT_TYPE_SIMON){
		Simon->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		Simon->SetAnimationSet(ani_set);
	}else if(object_type == OBJECT_TYPE_WHIP){
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		Simon->whip = (CWhip*)obj;
	}else if(object_type == OBJECT_TYPE_KNIFE){
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		Simon->knife = (CKnife*)obj; 
	}else if(object_type == OBJECT_TYPE_BOOMERANG){
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		Simon->boomerang = (CBoomerang*)obj; 
	}else if(object_type == OBJECT_TYPE_WHIP){
		// DebugOut(L"[INFO] This is your happy commandabc: \n"); 

		// obj->SetPosition(x, y);
		// LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		// obj->SetAnimationSet(ani_set);
		// mario->whip = (CWhip*)obj;
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
	this->Simon = CSimon::GetInstance();
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

		if(line == "[TILEMAP_IMAGE]"){ 
			section = SCENE_SECTION_TILEMAP_IMAGE; continue;
		}
		if(line == "[TILEMAP_TXT]"){ 
			section = SCENE_SECTION_TILEMAP_MAPTXT; continue; 
		}

		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		// if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch(section){
			case SCENE_SECTION_TILEMAP_IMAGE: _ParseSection_TILEMAP_IMAGE(line); break;
			case SCENE_SECTION_TILEMAP_MAPTXT: _ParseSection_TILEMAP_MAPTXT(line); break; 

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

	Simon->Update(dt,&coObjects);
	
	// Update camera to follow mario
	float cx, cy;
	Simon->GetPosition(cx, cy);
	//Update Simon position

	int a = CGame::GetInstance()->current_scene;
	// DebugOut(L"[INFO] This is your current scene: %d\n", a);
	// DebugOut(L"[INFO] Position of Simon X: %f\n", cx);
	// DebugOut(L"[INFO] Position of Simon Y: %f\n", cy);
	if(a==2){
		// if(cx<=70){
		// 	mario->SetPosition(70,cy);
		// }else if(cx>=407){
		// 	mario->SetPosition(407,cy);
		// }
		// cx=255;
	}else if(a==1){
		if(cx<=0){
			Simon->SetPosition(0,cy);
		}else if(cx>=1369){
			Simon->SetPosition(1369,cy);
		}
	//	Update camera position
		if( cx<0 || (cx>=0 && cx<=255) ){
			cx=255;
		}else if(cx>=1259){
			cx=1259;
		}
	}else if(a==4){ // scene 2.2
		if(cx<=0){
			Simon->SetPosition(0,cy);
		}else if(cx>=920){
			Simon->SetPosition(920,cy);
		}
		//Update camera position
		if( cx<0 || (cx>=0 && cx<=255) ){
			cx=255;
		}else if(cx>=770){
			cx=770;
		}
	}else if(a==5){ // scene 3.1
		if(cx<=0){
			Simon->SetPosition(0,cy);
		}else if(cx>=1430){
			Simon->SetPosition(1430,cy);
		}
		//Update camera position
		if( cx<0 || (cx>=0 && cx<=250) ){
			cx=250;
		}else if(cx>=1285){
			cx=1285;
		}
	}

	CGame *game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx,0);
	
}

void CPlayScene::Render(){
	map->renderMap();

	for (int i = 0; i < objects.size(); i++){
		objects[i]->Render();
	}
	Simon->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	Simon = NULL;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode){
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CSimon *simon = CSimon::GetInstance();
	switch (KeyCode)
	{
	case DIK_SPACE:
		if(simon->isSit == true 
		  || simon->isUsedSubWeaponKnife == true
		  || simon->isTakeWeapon == true) 
			return;
		if(simon->isJumped == false){
			simon->SetState(MARIO_STATE_JUMP);
		}
		break;
	case DIK_A: // reset
		if(simon->isUsedWhip == true 
		   || simon->isAttack == true
		   || simon->state == SIMON_STATE_ATTACK_STAND_RIGHT
		   || simon->isWalking == true
		   || simon->isUsedSubWeaponKnife == true
		   || simon->isTakeWeapon == true){
			break;
			DebugOut(L"[INFO] Phim attack bi can lien tuc \n");
		}else{
			simon->isUsedWhip = true;

			if (simon->nx > 0){
				simon->SetState(SIMON_STATE_ATTACK_STAND_RIGHT);
			}else{
				simon->SetState(SIMON_STATE_ATTACK_STAND_LEFT);
			}
		}
		break;
	case DIK_S:
		if(simon->isUsedSubWeaponKnife == true
		  || simon->canUseKnife == false
		  || simon->isTakeWeapon == true) break;
		  
		simon->isUsedSubWeaponKnife = true;
		if (simon->nx > 0){
			simon->SetState(SIMON_STATE_ATTACK_STAND_RIGHT);
		}else{
			simon->SetState(SIMON_STATE_ATTACK_STAND_LEFT);
		}
		break;
	case DIK_D:
		if(simon->isUsedSubWeaponBoomerang == true
		||  simon->isUsedSubWeaponKnife== true
		||	simon->isTakeWeapon == true) return;

		simon->isUsedSubWeaponBoomerang = true;
		if (simon->nx > 0){
			simon ->SetState(SIMON_STATE_ATTACK_STAND_RIGHT);
		}else{
			simon->SetState(SIMON_STATE_ATTACK_STAND_LEFT);
		}
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode){
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	CSimon *simon = CSimon::GetInstance();
	switch (KeyCode) {
		case DIK_DOWN:
			
			if (simon->isAttack && simon->isUsedWhip){
				simon->animation_set->at(SIMON_ANI_ATTACK_STAND_RIGHT)->SetCurrentFrame(-1);
				simon->animation_set->at(SIMON_ANI_ATTACK_STAND_LEFT)->SetCurrentFrame(-1);
				simon->whip->animation_set->at(ANIMATION_ATTACK_WHIP_RIGHT_ZERO)->SetCurrentFrame(-1);
				simon->whip->animation_set->at(ANIMATION_ATTACK_WHIP_LEFT_ZERO)->SetCurrentFrame(-1);
				simon->attackTime = 0.0f;
			}else{
				simon->isSit = !simon->isSit;
			}
			
			// marioo->isSit = false;
			// marioo->isAttack = false;
			// marioo->isUsedWhip = false;
			break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states){
	CGame *game = CGame::GetInstance();
	CSimon *simon = CSimon::GetInstance();

	// disable control key when simon  die 
	if (simon->GetState() == SIMON_STATE_DIE
		|| simon->isUsedSubWeaponKnife == true
		|| simon->isTakeWeapon == true)
	{
		return;
	}
	if(game->IsKeyDown(DIK_RIGHT) && !simon->isSit  && !simon->isAttack && !game->IsKeyDown(DIK_A)){
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}else if (game->IsKeyDown(DIK_LEFT) && !simon->isSit && !simon->isAttack && !game->IsKeyDown(DIK_A)){
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	}else if(game->IsKeyDown(DIK_DOWN) && !simon->isAttack && !simon->isJumped){
		// marioo->isSit = true;
		simon->SetState(SIMON_STATE_SIT_RIGHT);
	}else{
		simon->SetState(SIMON_STATE_IDLE);
	}
}