#include "Map.h"
#include "Textures.h"
#include "Sophia.h"

Map* Map::_instance = NULL;

Map::Map()
{
}

void Map::Render()
{
	Textures* textures = Textures::GetInstance();
	LPDIRECT3DTEXTURE9 texMap;
	texMap = textures->Get(texID);
	Sprites* sprites = Sprites::GetInstance();
	sprites->Add(99999, 0, 0, width, height, texMap);

	Sprite* sprite = sprites->Get(99999);

	RECT r; // bounding box of title red
	r.left = 0;
	r.top =  0;
	r.right = 0;
	r.bottom = 0;
	//if (IsCollision(cam->GetBound(), r)) // if title rect touch the Camera rect then do not draw 
	sprite->Draw(r.left, r.top);


}

void Map::Update(float dt)
{
}
Map* Map::GetInstance()
{
	if (_instance == NULL)
		_instance = new Map();
	return _instance;
}
Map::~Map()
{
}