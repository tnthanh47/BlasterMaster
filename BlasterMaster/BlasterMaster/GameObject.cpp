#include <d3dx9.h>
#include <algorithm>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Textures.h"



GameObject::GameObject(){
	x = y = 0;
	vx = vy = 0;
	nx = 1;
}

void GameObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

LPCOLLISIONEVENT GameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	Game::SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CollisionEvent* e = new CollisionEvent(t, nx, ny, coO);
	return e;
}

void GameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CollisionEvent::compare);
}

void GameObject::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

bool GameObject::CollisionWithObject(GameObject* obj)	// kt  AABB + Sweept AABB
{
	if (obj == NULL)
		return false;
	else
	{
		float l1, l2, b1, b2, r1, r2, t1, t2;
		this->GetBoundingBox(l1, t1, r1, b1);
		obj->GetBoundingBox(l2, t2, r2, b2);
		if (Game::GetInstance()->CheckAABB(l1, t1, r1, b1, l2, t2, r2, b2))
			return true;
		LPCOLLISIONEVENT e = SweptAABBEx(obj);
		bool res = e->t > 0 && e->t <= 1.0f;
		SAFE_DELETE(e);
		return res;
	}
}

void GameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = Textures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;
	//Game::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, 80);
}

RECT GameObject::GetRect()
{
	float l, t, r, b;
	RECT rect;
	GetBoundingBox(l, t, r, b);
	rect.left = l;
	rect.top = t;
	rect.right = r;
	rect.bottom = b;
	return rect;

}

GameObject::~GameObject(){
}
		
