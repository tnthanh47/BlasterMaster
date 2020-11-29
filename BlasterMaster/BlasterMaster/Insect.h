#pragma once
#include "GameObject.h"
#include "Sophia.h"

class CInsect : public GameObject
{


public:
	bool isWalk;
	TYPE typeEnemy;
	Animation* CurAnimation;
	CInsect();
	STATEOBJECT StateObject;
	void ChangeAnimation(STATEOBJECT StateObject);
	STATEOBJECT GetStateObject() { return this->StateObject; }
	void SetType(TYPE type) { this->typeEnemy = type; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void Reset();
};