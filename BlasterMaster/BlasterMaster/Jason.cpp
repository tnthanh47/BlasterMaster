#include <algorithm>
#include <assert.h>


#include "Jason.h"
#include "Game.h"
#include "Portal.h"
#include "Camera.h"
#include "Brick.h"

#include "PlayerState.h"
#include "PlayerFallingState.h"
#include "PlayerJumpingState.h"
#include "PlayerJumpingMovingState.h"
#include "PlayerUpwardState.h"
#include "PlayerUpwardJumpingState.h"
#include "PlayerUpwardJumpingMovingState.h"
#include "PlayerMovingState.h"
#include "PlayerStandingState.h"
#include "PlayerOpenState.h"

#include "BulletMovingState.h"

Jason* Jason::_instance = NULL;

Jason::Jason() {
	IsUp = false;
	IsJumping = false;
	playerType = JASON;
}

Jason::~Jason() {

}

void Jason::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	GameObject::Update(dt);

	// Simple fall down

	vy += JASON_GRAVITY * dt;

	if(player->allow[JASON])
		state->Update();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	/*for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Update(dt, coObjects);
	}
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->GetStateObject() == BULLET_SMALL_HIT) {
			bullets.erase(bullets.begin() + i);
		}
	}*/

	// turn off collision when die 

	CalcPotentialCollisions(coObjects, coEvents);



	// No collision occured, proceed normally
	
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		// block 

		x += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.3f;

		// Collision logic with Enemies

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Sophia*>(e->obj)) {
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}

			if (dynamic_cast<Brick*>(e->obj)) {
				if (e->nx != 0) {

					if (!IsJumping) {
						 vx = 0;
					}
					else  {
						if (this->nx == 1)
							vx = JASON_MOVING_SPEED;
						else
							vx = -JASON_MOVING_SPEED;
					}
				}
				if (e->ny == -1)
				{
						vy = 0;
						IsJumping = false;
				}
				else if (e->ny == 1)
				{
						vy = 0;
						ChangeAnimation(new PlayerFallingState());
				}
			}

			if (dynamic_cast<Portal*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				Portal* p = dynamic_cast<Portal*>(e->obj);
				IsTouchPortal = true;
				scene_id = p->scene_id;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void Jason::ChangeAnimation(PlayerState* newState, int stateChange) {
	delete state;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	state = newState;
	LPANIMATION_SET animationSet = animation_sets->Get(playerType);
	CurAnimation = animationSet->Get(newState->StateName);
}

void Jason::Render() {
	int alpha = 255;
	if (IsRender) {
		CurAnimation->Render(x, y, alpha, idFrame, RenderOneFrame);
		RenderBoundingBox();
	}

	/*for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->Render();
	}*/
}

void Jason::GetBoundingBox(float& left, float& top, float& right, float& bottom){
	left = x;
	top = y;

	if (stateBoundingBox == JASON_BOUNDING_BOX) {
		right = x + JASON_BBOX_WIDTH;
		bottom = y + JASON_BBOX_HEIGHT;
	}
}

Jason* Jason::GetInstance() {
	if (_instance == NULL) {
		_instance = new Jason();
	}
	return _instance;
}

void Jason::OnKeyDown(int key) {
	switch (key) {
	case DIK_SPACE:
		ChangeAnimation(new PlayerJumpingState());
		break;
	case DIK_3:
		SetPosition(565, 112);
		break;
	case DIK_Q: // get on the car
		if (player->allow[JASON] && (x >= player->x + (SOPHIA_BBOX_WIDTH / 3)||x<= player->x + SOPHIA_BBOX_WIDTH)) {
			IsRender = false;
			player->IsOpen = false;
			player->allow[JASON] = false;
			player->allow[SOPHIA] = true;
			player->y = player->y + (SOPHIA_OPEN_BBOX_HEIGHT - SOPHIA_BBOX_HEIGHT);
			player->ChangeAnimation(new PlayerStandingState());
			break;
		}
	case DIK_S:
		break;
	}
}

void Jason::OnKeyUp(int key) {
	
}

void Jason::Reset(float x, float y) {
	nx = 1;
	SetPosition(x, y);
	ChangeAnimation(new PlayerStandingState());
	SetSpeed(0, 0);
}

void Jason::Fire() {
	bullet = new Bullet();
	bullet->typeBullet = BULLET_SMALL;
	if (!IsUp) {
		if (nx > 0) {
			bullet->SetPosition(x + SOPHIA_BBOX_WIDTH, y + 7 / SOPHIA_BBOX_HEIGHT);
			bullet->ChangeAnimation(BULLET_SMALL_MOVING_RIGHT);
		}
		else {
			bullet->SetPosition(x, y + 7 / SOPHIA_BBOX_HEIGHT);
			bullet->ChangeAnimation(BULLET_SMALL_MOVING_LEFT);
		}
	}
	else {
		if (nx != 0) {
			bullet->SetPosition(x + SOPHIA_BBOX_WIDTH / 3, y);
			bullet->ChangeAnimation(BULLET_SMALL_MOVING_UP);
		}
	}

	if (bullets.size() < 3) {
		bullets.push_back(bullet);
	}
}

void Jason::DeleteBullet() {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->GetX() >= x + 150.0f || bullets[i]->GetX() <= x - 150.0f) {
			bullets.erase(bullets.begin() + i);
		}
		else if (bullets[i]->GetY() <= y - 100.0f) {
			bullets.erase(bullets.begin() + i);
		}
		else if (bullets[i]->GetStateObject() == BULLET_SMALL_HIT) {
			bullets.erase(bullets.begin() + i);
		}
	}
}