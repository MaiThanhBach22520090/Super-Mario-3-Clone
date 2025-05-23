#include "Leaf.h"
#include "PlayScene.h"

CLeaf::CLeaf(float x, float y) : CGameObject(x, y)
{
	this->x = x;
	this->y = y;
	this->vx = 0;
	this->vy = LEAF_FLOAT_SPEED;
	this->ay = LEAF_GRAVITY;
	originX = x;
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
    if (goingUp)
    {
        vy += ay * dt;
        if (vy >= 0)  // reached apex
        {
            goingUp = false;
            vy = LEAF_FALL_SPEED;
            swayStartTime = GetTickCount64(); // start sway timer
        }
    }
    else
    {
        // Swaying horizontally in sinusoidal style
        DWORD now = GetTickCount64();
        float timeSinceSway = (now - swayStartTime) / 1000.0f;

        // Use sine wave for smoother side-to-side motion
        vx = LEAF_SWAY_SPEED * sinf(timeSinceSway * 2.0f * 3.14159f / LEAF_SWAY_PERIOD);

        // Vertical fall speed remains mostly constant
        vy = LEAF_FALL_SPEED;
    }

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

