#include "PlayerStandingState.h"
#include "PlayerMovingState.h"
#include "PlayerUpwardState.h"
#include "PlayerTurningState.h"

PlayerStandingState::PlayerStandingState() {
	if (Allow[SOPHIA]) {
		player->vx = 0;
		player->IsMoving = false;
		player->RenderOneFrame = true;
		player->RenderBack = false;

		if (player->nx > 0) {
			StateName = SOPHIA_STANDING_RIGHT;
		}
		else {
			StateName = SOPHIA_STANDING_LEFT;
		}
		player->stateBoundingBox = SOPHIA_BOUNDING_BOX;
	}
	else if (Allow[JASON]) {
		playerSmall->vx = 0;
		playerSmall->vy = 0;

		if (playerSmall->nx > 0) {
			StateName = JASON_STANDING_RIGHT;
		}
		else {
			StateName = JASON_STANDING_LEFT;
		}
		playerSmall->stateBoundingBox = JASON_BOUNDING_BOX;
	}
	else if (Allow[BIG_JASON]) {
		playerBig->vx = 0;
		playerBig->vy = 0;
		if (playerBig->nx > 0) {
			StateName = BIG_JASON_STANDING_RIGHT;
		}
		else if (playerBig->nx < 0) {
			StateName = BIG_JASON_STANDING_LEFT;
		}
		else if (playerBig->ny > 0) {
			StateName = BIG_JASON_STANDING_UP;
		}
		else if (playerBig->ny < 0) {
			StateName = BIG_JASON_STANDING_DOWN;
		}
		playerBig->stateBoundingBox = BIG_JASON_BOUNDING_BOX;
	}
}

PlayerStandingState::~PlayerStandingState() {

}

void PlayerStandingState::Update() {

	this->HandleKeyboard();
}

void PlayerStandingState::HandleKeyboard() {
	if (keyCode[DIK_LEFT] && keyCode[DIK_RIGHT]) {
		if (Allow[SOPHIA])
			player->ChangeAnimation(new PlayerStandingState(), NORMAL);
		else if (Allow[JASON])
			playerSmall->ChangeAnimation(new PlayerStandingState());
		else if(Allow[BIG_JASON])
			playerBig->ChangeAnimation(new PlayerStandingState());
	}
	else if (keyCode[DIK_LEFT]) {
		if (Allow[SOPHIA]) {
			if (player->nx < 0) {
				player->nx = -1;
				player->ChangeAnimation(new PlayerMovingState(), STAND_TO_MOVE);
			}
			else
			{
				player->ChangeAnimation(new PlayerTurningState(), NORMAL);
				player->CurAnimation->currentFrame = -1;
				player->CurAnimation->isLastFrame = false;
			}
		}
		else if (Allow[JASON]) {
			playerSmall->ChangeAnimation(new PlayerMovingState());
		}
		else if (Allow[BIG_JASON]) {
			playerBig->ChangeAnimation(new PlayerMovingState());
		}

	}
	else if (keyCode[DIK_RIGHT]) {
		if (Allow[SOPHIA]) {
			if (player->nx > 0) {
				player->nx = 1;
				player->ChangeAnimation(new PlayerMovingState(), STAND_TO_MOVE);
			}
			else
			{
				player->ChangeAnimation(new PlayerTurningState(), NORMAL);
				player->CurAnimation->currentFrame = -1;
				player->CurAnimation->isLastFrame = false;
			}
		}
		else if (Allow[JASON]) {
			playerSmall->ChangeAnimation(new PlayerMovingState());
		}
		else if (Allow[BIG_JASON]) {
			playerBig->ChangeAnimation(new PlayerMovingState());
		}

	}
	else if (keyCode[DIK_UPARROW]) {
		if (Allow[SOPHIA])
			player->ChangeAnimation(new PlayerUpwardState());
		if (Allow[BIG_JASON]) {
			playerBig->ChangeAnimation(new PlayerMovingState());
		}
	}
	else if (keyCode[DIK_DOWNARROW]) {
		if (Allow[BIG_JASON]) {
			playerBig->ChangeAnimation(new PlayerMovingState());
		}
	}
}