#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "SolidBlock.h"
#include "BackgroundObject.h"
#include "Mushroom.h"
#include "MysteryBox.h"
#include "PiranhaPlant.h"
#include "Koopa.h"
#include "Paratroopa.h"
#include "Leaf.h"
#include "BackPlatform.h"
#include "Ground.h"
#include "Cloud.h"
#include "GoldenBrick.h"
#include "TeleportTunnel.h"
#include "GUI.h"


#include "SampleKeyEventHandler.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

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

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
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

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		// Input format: object_type x y has_wings
		if (tokens.size() < 4) return;
		int Paragoomba = atoi(tokens[3].c_str());
		bool hasWings = Paragoomba == 1 ? true : false;
		obj = new CGoomba(x, y, hasWings);
		break;
	}
	case OBJECT_TYPE_BRICK: obj = new CBrick(x,y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y, false); break;
	case OBJECT_TYPE_KOOPA:	obj = new CKoopa(x, y, false);	break;
	case OBJECT_TYPE_PARATROOPA: obj = new CKoopa(x, y, true); break;
	case OBJECT_TYPE_LEAF:	obj = new CLeaf(x, y);	break;

	case OBJECT_TYPE_BRICK_ARRAY:
	{

		// Input format: object_type x y arr_size gap sprite_id IsHorizontal
		if (tokens.size() < 7) return;

		int arr_size = atoi(tokens[3].c_str());
		int gap = atoi(tokens[4].c_str());
		int sprite_id = atoi(tokens[5].c_str());
		float cellWidth = 16;
		float cellHeight = 16;
		int IsHorizontal = atoi(tokens[6].c_str());

		if (IsHorizontal == 1)
		{
			for (int i = 0; i < arr_size; i++)
			{
				CGameObject* obj = new cSolidBlock(x + i * (cellWidth + gap), y, sprite_id, cellWidth, cellHeight);
				obj->SetPosition(x + i * (cellWidth + gap), y);
				objects.insert(objects.begin(), obj);
			}
		}
		else
		{
			for (int i = 0; i < arr_size; i++)
			{
				CGameObject* obj = new cSolidBlock(x, y + i * (cellHeight + gap), sprite_id, cellWidth, cellHeight);
				obj->SetPosition(x, y + i * (cellHeight + gap));
				objects.insert(objects.begin(), obj);
			}
		}

		return; // Skip the general object setup for brick array
	}

	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, true
		);

		break;
	}

	case OBJECT_TYPE_BACK_PLATFORM:
	{
		//Format: object_type x y cell_width cell_height length height sprite_base_id
		if (tokens.size() < 8) return;
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());

		int length = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());

		int sprite_base_id = atoi(tokens[7].c_str());
		if (length <= 0 || height <= 0) return;
		if (cell_width <= 0 || cell_height <= 0) return;

		// Create the back platform object
		obj = new BackPlatform(
			x, y,
			cell_width, cell_height, length, height,
			sprite_base_id
		);

		objects.push_back(obj);

		return;
	}

	case OBJECT_TYPE_BIG_BUSH:
	{
		// Format: object_type x y height top_left top_right body_left body_right
		if (tokens.size() < 8) return;

		int height = atoi(tokens[3].c_str());
		int top_left = atoi(tokens[4].c_str());
		int top_right = atoi(tokens[5].c_str());
		int body_left = atoi(tokens[6].c_str());
		int body_right = atoi(tokens[7].c_str());

		int cellWidth = 16;
		int cellHeight = 16;

		// Top
		CGameObject* top_left_obj = new CBackgroundObject(x, y, top_left);
		CGameObject* top_right_obj = new CBackgroundObject(x + cellWidth, y, top_right);
		top_left_obj->SetPosition(x, y);
		top_right_obj->SetPosition(x + cellWidth, y);
		objects.insert(objects.begin(), top_right_obj);
		objects.insert(objects.begin(), top_left_obj);

		if (height < 2) return;

		// Body
		for (int i = 1; i < height; i++)	
		{
			float body_y = y + i * cellHeight;

			CGameObject* body_left_obj = new CBackgroundObject(x, body_y, body_left);
			CGameObject* body_right_obj = new CBackgroundObject(x + cellWidth, body_y, body_right);
			body_left_obj->SetPosition(x, body_y);
			body_right_obj->SetPosition(x + cellWidth, body_y);
			objects.insert(objects.begin(), body_right_obj);
			objects.insert(objects.begin(), body_left_obj);
		}

		return;
	}
	
	case OBJECT_TYPE_MUSHROOM:
	{
		if (tokens.size() < 4) return;
		int animation_id = atoi(tokens[3].c_str());

		obj = new CMushroom(x, y, animation_id);
		break;
	}

	case OBJECT_TYPE_MYSTERY_BOX:
	{
		if (tokens.size() < 6) return;
		int animation_id_active = atoi(tokens[3].c_str());
		int animation_id_used = atoi(tokens[4].c_str());
		int spawn_object_id = atoi(tokens[5].c_str());

		obj = new CMysteryBox(x, y, animation_id_active, animation_id_used, spawn_object_id);

		objects.push_back(obj);
		break;
	}
	
	case OBJECT_TYPE_PIRANHA_PLANT:
	{
		if (tokens.size() < 4) return;
		bool isShootFire = atoi(tokens[3].c_str()) == 1;
		obj = new CPiranhaPlant(x, y, isShootFire);
		break;
	}

	case OBJECT_TYPE_SMALL_BUSH:
	{
		// Input format: object_type x y lenght sprite_id
		if (tokens.size() < 5) return;
		int length = atoi(tokens[3].c_str());
		int sprite_id = atoi(tokens[4].c_str());
		int cellWidth = 16;
		int cellHeight = 16;

		for (int i = 0; i < length; i++)
		{
			CGameObject* obj = new CBackgroundObject(x + i * cellWidth, y, sprite_id);
			obj->SetPosition(x + i * cellWidth, y);
			objects.insert(objects.begin(), obj);
		}
	}

	case OBJECT_TYPE_GROUND:
	{
		// Input format: object_type x y cell_width cell_height length height sprite_base_id
		if (tokens.size() < 8) return;
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int height = atoi(tokens[6].c_str());
		int sprite_base_id = atoi(tokens[7].c_str());
		if (length <= 0 || height <= 0) return;
		if (cell_width <= 0 || cell_height <= 0) return;
		CGameObject* obj = new CGround(
			x, y,
			cell_width, cell_height, length, height,
			sprite_base_id
		);

		// Add the ground object to the scene
		objects.push_back(obj);
		return; // Skip the general object setup for ground

	}

	case OBJECT_TYPE_CLOUD:
	{
		// Input format: object_type x y length sprite_base_id
		if (tokens.size() < 5) return;
		int length = atoi(tokens[3].c_str());
		int sprite_base_id = atoi(tokens[4].c_str());
		if (length <= 0) return;
		CGameObject* obj = new CCloud(x, y, length, sprite_base_id);
		objects.push_back(obj);
		return; // Skip the general object setup for cloud
	}

	case OBJECT_TYPE_GOLDEN_BRICK:
	{
		// Input format: object_type x y has_button
		/*if (tokens.size() < 4) return;
		bool hasButton = atoi(tokens[3].c_str()) == 1 ? true : false;
		obj = new CGoldenBrick(x, y, hasButton);
		break;*/

		// Input format: object_type x y width length has_button;
		if (tokens.size() < 5) return;
		int width = atoi(tokens[3].c_str());
		int length = atoi(tokens[4].c_str());
		bool hasButton = atoi(tokens[5].c_str()) == 1 ? true : false;
		if (length <= 0) return;	
		
		// Create an array of golden bricks
		for (int i = 0; i < length; i++)
		{
			CGameObject* obj = new CGoldenBrick(x + i * width, y, hasButton);
			obj->SetPosition(x + i * width, y);
			objects.push_back(obj);
		}
		return;	
	}

	case OBJECT_TYPE_TELEPORT_TUNNEL:
	{
		// Input format: object_type x y width height target_scene_id
		if (tokens.size() < 6) return;
		int width = atoi(tokens[3].c_str());
		int height = atoi(tokens[4].c_str());
		bool isGoingDown = atoi(tokens[5].c_str()) == 1 ? true : false;
		int target_position_x = atoi(tokens[6].c_str());
		int target_position_y = atoi(tokens[7].c_str());
		obj = new CTeleportTunnel(
			x, y, width, height, isGoingDown,
			target_position_x, target_position_y
		);
		break;
	}

	case OBJECT_TYPE_TUNNEL:
	{
		// Input format: object_type x y height entrance_left entrance_right body_left body_right
		if (tokens.size() < 8) return;

		int height = atoi(tokens[3].c_str());
		int entrance_left = atoi(tokens[4].c_str());
		int entrance_right = atoi(tokens[5].c_str());
		int body_left = atoi(tokens[6].c_str());
		int body_right = atoi(tokens[7].c_str());
		float cellWidth = 16;
		float cellHeight = 16;

		// Entrance
		CGameObject* entrance_left_obj = new cSolidBlock(x, y, entrance_left, cellWidth, cellHeight);
		entrance_left_obj->SetPosition(x, y);
		objects.push_back(entrance_left_obj);
		CGameObject* entrance_right_obj = new cSolidBlock(x + cellWidth, y, entrance_right, cellWidth, cellHeight);
		objects.push_back(entrance_right_obj);

		// Body
		if (height < 2) return;
		for (int i = 1; i < height; i++)
		{
			CGameObject* body_left_obj = new cSolidBlock(x, y + i * cellHeight, body_left, cellWidth, cellHeight);
			body_left_obj->SetPosition(x, y + i * cellHeight);
			objects.push_back(body_left_obj);
			CGameObject* body_right_obj = new cSolidBlock(x + cellWidth, y + i * cellHeight, body_right, cellWidth, cellHeight);
			objects.push_back(body_right_obj);
		}
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}

	case OBJECT_TYPE_GUI:
	{
		// Input format: object_type x y
		if (tokens.size() < 3) return;
		obj = new CGUI(x, y);
		break;
	}

	break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Get target player Y position
	float px, py;
	player->GetPosition(px, py);

	CGame* game = CGame::GetInstance();

	// Fixed X camera (center screen)
	float camX = px - game->GetBackBufferWidth() / 2;
	if (camX < 0) camX = 0;

	// Determine if Mario is in a hidden room (e.g., Y > 448)
	isInHiddenRoom = py > 440;

	// Default: follow Mario, keeping him 2/3 up the screen
	float targetCamY = py - game->GetBackBufferHeight() * 2 / 3;

	// Clamp for normal areas
	if (!isInHiddenRoom)
	{
		if (targetCamY < 0) targetCamY = 0;
		if (targetCamY > 50) targetCamY = 224; // Normal max Y
	}
	else
	{
		// If in hidden room, lock to a fixed Y camera value
		targetCamY = 440; // Fixed Y position for hidden room
	}


	// Get current cam Y
	float currentCamX, currentCamY;
	game->GetCamPos(currentCamX, currentCamY);

	// Smoothly interpolate Y
	const float SMOOTH_Y = 0.05f;
	float camY;
	if (!isInHiddenRoom) camY = currentCamY + (targetCamY - currentCamY) * SMOOTH_Y;
	else camY = targetCamY;

	// Apply camera position
	game->SetCamPos(camX, camY);

	PurgeDeletedObjects();
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}