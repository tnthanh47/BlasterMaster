#include "PlayerUpwardJumpingMovingState.h"
#include "PlayerUpwardJumpingState.h"

PlayerUpwardJumpingState::PlayerUpwardJumpingState() {

	player->IsUp = true;
	player->renderOneFrame = true;

	if (!player->IsJumping) {
		player->vy = -SOPHIA_JUMPING_SPEED_Y;
	}
	player->IsJumping = true;
	if (player->nx > 0) {
		StateName = UPWARD_MOVING_RIGHT;
	}
	else {
		StateName = UPWARD_MOVING_LEFT;
	}
	player->stateBoundingBox = SOPHIA_UP_BOUNDING_BOX;
}

PlayerUpwardJumpingState::~PlayerUpwardJumpingState() {

}

void PlayerUpwardJumpingState::Update() {
	this->HandleKeyboard();
}

void PlayerUpwardJumpingState::HandleKeyboard() {
	if (keyCode[DIK_RIGHT]) {
		player->nx = 1;
		player->ChangeAnimation(new PlayerUpwardJumpingMovingState(), NORMAL);
	}
	else if (keyCode[DIK_LEFT]) {
		player->nx = -1;
		player->ChangeAnimation(new PlayerUpwardJumpingMovingState(), NORMAL);
	}
}