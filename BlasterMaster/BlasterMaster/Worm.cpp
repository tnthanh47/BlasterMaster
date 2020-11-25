#include "Worm.h"
#include "Sophia.h"
#include "Jason.h"
#include "Brick.h"
#include "Camera.h"

CWorm::CWorm(float x, float y)
{
	this->x = x;
	this->y = y;
	typeEnemy = WORM;
	Reset();

}

void CWorm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WORM_BBOX_WIDTH;
	bottom = y + WORM_BBOX_HEIGHT;
}

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (Allow[JASON] || Allow[SOPHIA]) {
	
		GameObject::Update(dt, coObjects);

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();

		float cx, cy;
		float kc1 = 0;
		float kc2 = 0;
		if (Allow[SOPHIA])
			player->GetPosition(cx, cy);
		else if (Allow[JASON])
			playerSmall->GetPosition(cx, cy);

		//DebugOut(L"health: %d\n", health);
		if (isInArea && StateObject != ENEMY_DEAD) {
			vy += WORM_GRAVITY * dt;
			if (x - cx > 0)
				ChangeAnimation(WORM_STATE_WALKING_LEFT);
			else
				ChangeAnimation(WORM_STATE_WALKING_RIGHT);
		}

		if (health <= 0) {
			StateObject = ENEMY_DEAD;
		//	DebugOut(L"imherer\n");
		}

		// turn off collision when die 
		//if (state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

		

		// update
		//kc = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
		kc1 = SCREEN_WIDTH - (cx - Camera::GetInstance()->GetCamPosX());
		kc2 = cx - Camera::GetInstance()->GetCamPosX();
		if (StateObject != ENEMY_DEAD) {
			if (abs(cx - x) <= kc1 && player->scene_id == 1)
				isInArea = true;
			else if (cx - x <= kc2 && player->scene_id == 2)
				isInArea = true;

			if (coEvents.size() == 0)
			{
				x += dx;
				y += dy;
			}
			else
			{
				float min_tx, min_ty, nx = 0, ny;
				float rdx = 0;
				float rdy = 0;

				// TODO: This is a very ugly designed function!!!!
				FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

				// block every object first!
				x += min_tx * dx + nx * 0.1f;
				y += min_ty * dy + ny * 0.1f;

				//
				// Collision logic with other objects
				//
				for (UINT i = 0; i < coEventsResult.size(); i++)
				{
					LPCOLLISIONEVENT e = coEventsResult[i];

					if (dynamic_cast<Brick*>(e->obj)) // if e->obj is Brick
					{
						Brick* brick = dynamic_cast<Brick*>(e->obj);

						if (e->ny != 0) vy = 0;
					}
				}
			}

			// clean up collision events
			
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	DebugOut(L"State: %d\n", StateObject);
}

void CWorm::Render()
{
	int alpha = 255;
	CurAnimation->Render(x, y, alpha);
	RenderBoundingBox();
}

void CWorm::ChangeAnimation(STATEOBJECT StateObject) {
	this->StateObject = StateObject;
	AnimationSets* animation_sets = AnimationSets::GetInstance();
	LPANIMATION_SET animationSet = animation_sets->Get(typeEnemy);
	CurAnimation = animationSet->Get(this->StateObject);
	switch (this->StateObject)
	{
	case WORM_STATE_WALKING_LEFT:
		vx = -WORM_WALKING_SPEED;
		//vy = 0;
		break;
	case WORM_STATE_WALKING_RIGHT:
		vx = WORM_WALKING_SPEED;
		//vy = 0;
		break;
	case WORM_STATE_IDLE_LEFT:
		vx = 0;
		vy = 0;
		break;
	case WORM_STATE_IDLE_RIGHT:
		vx = 0;
		vy = 0;
		break;
	default:
		break;
	}
}

void CWorm::Reset() {
	//DebugOut(L"Imhere");
	nx = -1;
	if (player->nx > 0)
		ChangeAnimation(WORM_STATE_IDLE_LEFT);
	else
		ChangeAnimation(WORM_STATE_IDLE_RIGHT);
	isIdle = true;
	isDrop = true;
	isWalking = false;
}
