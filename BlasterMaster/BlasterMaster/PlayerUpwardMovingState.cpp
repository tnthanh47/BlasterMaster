#include "PlayerUpwardMovingState.h"
#include "PlayerUpwardState.h"
#include "PlayerStandingState.h"

PlayerUpwardMovingState::PlayerUpwardMovingState() {

	player->RenderOneFrame = false;
	player->IsUp = true;

	if (player->nx > 0) {
		StateName = SOPHIA_UPWARD_MOVING_RIGHT;
		player->vx = SOPHIA_MOVING_SPEED;
	}
	else {
		StateName = SOPHIA_UPWARD_MOVING_LEFT;
		player->vx = -SOPHIA_MOVING_SPEED;
	}

	player->stateBoundingBox = SOPHIA_UP_BOUNDING_BOX;
}

PlayerUpwardMovingState::~PlayerUpwardMovingState() {
	if (!player->IsJumping) {
		player->ChangeAnimation(new PlayerUpwardMovingState(), NORMAL);
		player->vx = 0;
		player->RenderOneFrame = true;
		return;
	}
}

void PlayerUpwardMovingState::Update() {
	this->HandleKeyboard();
}

void PlayerUpwardMovingState::HandleKeyboard() {
	if (keyCode[DIK_RIGHT]) {
		player->nx = 1;
		player->ChangeAnimation(new PlayerUpwardMovingState());
	}
	else if (keyCode[DIK_LEFT]) {
		player->nx = -1;
		player->ChangeAnimation(new PlayerUpwardMovingState());
	}
}