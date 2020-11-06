#include "PlayerJumpTurningState.h"
#include "PlayerFallingState.h"

PlayerJumpTurningState::PlayerJumpTurningState() {
	player->RenderBack = false;
	player->RenderOneFrame = false;

	if (player->nx > 0) {
		player->nx = -1;
		player->vx = -SOPHIA_MOVING_SPEED;
		player->RenderBack = false;
		StateName = static_cast<STATENAME>(player->idFrame + 22);
	}
	else {
		player->nx = 1;
		player->vx = SOPHIA_MOVING_SPEED;
		player->RenderBack = true;
		StateName = static_cast<STATENAME>(25 - player->idFrame);
	}

	player->stateBoundingBox = SOPHIA_BOUNDING_BOX;
}

PlayerJumpTurningState::~PlayerJumpTurningState() {

}

void PlayerJumpTurningState::Update() {
	if (player->nx > 0) {
		for (int i = 0; i < 4; i++) {
			if (StateName == static_cast<STATENAME>(i + 22))
				player->idFrame = i;
		}
	}
	else {
		for (int i = 3; i >= 0; i--) {
			if (StateName == static_cast<STATENAME>(25 - i))
				player->idFrame = i;
		}
	}
	this->HandleKeyboard();
}

void PlayerJumpTurningState::HandleKeyboard() {
	if (player->CurAnimation->isLastFrame) {
		player->ChangeAnimation(new PlayerFallingState());
	}
}