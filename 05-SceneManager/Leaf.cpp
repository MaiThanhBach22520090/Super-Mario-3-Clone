#include "Leaf.h"
#include "PlayScene.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
	this->x = x;
	this->y = y;
	this->vx = 0;
	this->vy = 0;
	this->ay = 0;
	spawnTime = GetTickCount64();
}

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{
	int aniId = ID_ANI_LEAF;

	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(aniId)->Render(x, y);
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

