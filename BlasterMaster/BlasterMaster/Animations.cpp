#include "Animations.h"
#include "debug.h"
#include <cmath>


AnimationSets* AnimationSets::__instance = NULL;

void Animation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = Sprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new AnimationFrame(sprite, t);
	frames.push_back(frame);
}

void Animation::Render(float x, float y, int alpha, int idFrame, bool renderOneFrame, bool rev)
{
	DWORD now = GetTickCount();
	if (!renderOneFrame) {
		if (currentFrame <= -1)
		{
			currentFrame = 0;
			lastFrameTime = now;
		}
		else
		{
			DWORD t = frames[currentFrame]->GetTime();
			if (now - lastFrameTime > t)
			{
				currentFrame++;
				lastFrameTime = now;
				if (currentFrame == frames.size()) currentFrame = 0;
			}
		}
	}
	else {
		currentFrame = idFrame;
	}
	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

Animations* Animations::__instance = NULL;

Animations* Animations::GetInstance()
{
	if (__instance == NULL) __instance = new Animations();
	return __instance;
}

void Animations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION Animations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void Animations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

///// animation set
void AnimationSet::Add(int aniId, STATENAME StateName)
{
	LPANIMATION ani = Animations::GetInstance()->Get(aniId);
	animations[StateName] = ani;
}
void AnimationSet::Add(int aniId, STATEOBJECT StateObject)
{
	LPANIMATION ani = Animations::GetInstance()->Get(aniId);
	animations[StateObject] = ani;
}
LPANIMATION AnimationSet::Get(STATENAME NameState)
{
	return animations[NameState];
}
LPANIMATION AnimationSet::Get(STATEOBJECT type)
{
	return animations[type];
}

///// animations sets
AnimationSets::AnimationSets(){
}

AnimationSets* AnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new AnimationSets();
	return __instance;
}

LPANIMATION_SET AnimationSets::Get(TYPE type)
{
	return animation_sets[type];
}

void AnimationSets::Add(TYPE type, LPANIMATION_SET ani_set)
{
	animation_sets[type] = ani_set;
}




