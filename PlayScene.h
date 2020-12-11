#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "DamageBrick.h"
#include "Stair.h"
#include "Camera.h"
#include "HUD.h"
#include "Sophia.h"
#include "Jason.h"
#include "BigJason.h"
#include "Bullet.h"
#include "Portal.h"
#include "Gate.h"
#include "Orb1.h"
#include "Worm.h"
#include "Floater.h"
#include "Power.h"
#include "Dome.h"
#include "Jumper.h"
#include "Insect.h"

#include "Grid.h"
#include "GlobalConfig.h"

using namespace std;

class PlayScene : public Scene {
protected:
	Sophia* sophia;
	Jason* jason;
	Power* power;
	Camera* gameCamera;
	vector<Bullet*> bullets;
	vector<LPGAMEOBJECT> listObjects;
	vector<Enemy*> listEnemies;
	vector<Portal*> listPortals;
	vector<Gate*> listGates;
	vector<Item*> listItems;
	HUD* hud;
	//Sophia* sophia;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_CLEARANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_SWITCHSCENE(string line);
	void _ParseSection_MAP(string line);

	//Get stage objects' link and tilemap resources
	//void _ParseSection_SCENEFILEPATH(string line);
public:
	PlayScene(int id, LPCWSTR filePath);
	/*void LoadBaseObjects();
	void LoadBaseTextures();*/
	virtual void Load();
	virtual void Update(DWORD dt);
	void UpdateBullet(DWORD dt, vector<LPGAMEOBJECT> coObjects);
	virtual void Render();
	virtual void Unload();
	void ChangeScene();
	Sophia* GetSophia() { return sophia; }
	Jason* GetJason() { return jason; }
	bool randomItem(){
		return 0 + (rand() % (1 - 0 + 1)) == 1;
	}
	//friend class PlaySceneKeyHandler;
};

class PlaySceneKeyHandler : public SceneKeyHandler{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlaySceneKeyHandler(Scene* s) :SceneKeyHandler(s) {};
};
