#include "PiranhaPlant.h"
#include "Fireball.h"
#include "PlayScene.h"
#include "Game.h"
#include "Mario.h"

CPiranhaPlant::CPiranhaPlant(float x, float y) : CGameObject(x, y)
{
    this->startY = y;
    this->isMovingUp = true;
    this->isIdle = true;
    this->stateTimer = GetTickCount64();
    this->hasShotFire = false;
}

CPiranhaPlant::CPiranhaPlant(float x, float y, bool isShootFire) : CGameObject(x, y)
{
	this->startY = y;
	this->isMovingUp = true;
	this->isIdle = true;
	this->stateTimer = GetTickCount64();
	this->hasShotFire = false;
	this->isShootFire = isShootFire;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);

    if (isIdle)
    {
        HandleIdleState();
    }
    else
    {
        HandleMovement(dt);
    }
}

void CPiranhaPlant::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    if (!mario) return;

    float marioX, marioY;
    mario->GetPosition(marioX, marioY);

    int aniId = ID_ANI_PIRANHA_NORMAL;

    if (isShootFire)
    {
        bool marioLeft = marioX < x;
        bool marioAbove = marioY < y;

        if (marioLeft && marioAbove)
            aniId = ID_ANI_PIRANHA_UL;
        else if (marioLeft && !marioAbove)
            aniId = ID_ANI_PIRANHA_DL;
        else if (!marioLeft && marioAbove)
            aniId = ID_ANI_PIRANHA_UR;
        else
            aniId = ID_ANI_PIRANHA_DR;
    }

    animations->Get(aniId)->Render(x, y);
    RenderBoundingBox();
}

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - PIRANHA_BBOX_WIDTH / 2;
    t = y - PIRANHA_BBOX_HEIGHT;
    r = l + PIRANHA_BBOX_WIDTH;
    b = y;
}

void CPiranhaPlant::HandleIdleState()
{
    if (GetTickCount64() - stateTimer >= PIRANHA_IDLE_TIME)
    {
        isIdle = false;
        vy = isMovingUp ? -PIRANHA_MOVE_SPEED : PIRANHA_MOVE_SPEED;
    }
}

void CPiranhaPlant::HandleMovement(DWORD dt)
{
    y += vy * dt;

    if (isMovingUp && y <= startY - PIRANHA_SPRITE_HEIGHT)
    {
        y = startY - PIRANHA_SPRITE_HEIGHT;
        isMovingUp = false;
        isIdle = true;
        vy = 0;
        stateTimer = GetTickCount64();

        if (isShootFire && !hasShotFire)
            TryShootFireball();
    }
    else if (!isMovingUp && y >= startY)
    {
        y = startY;
        isMovingUp = true;
        isIdle = true;
        vy = 0;
        hasShotFire = false;
        stateTimer = GetTickCount64();
    }
}

void CPiranhaPlant::TryShootFireball()
{
    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();
    if (!mario) return;

    float marioX, marioY;
    mario->GetPosition(marioX, marioY);

    float dx = marioX - x;
    float dy = marioY - y;
    float len = sqrtf(dx * dx + dy * dy);
    if (len == 0) len = 1.0f;

    float vx = 0.15f * dx / len;
    float vy = 0.15f * dy / len;

    float fireballX = x;
    float fireballY = y - PIRANHA_BBOX_HEIGHT / 2;

    CFireball* fireball = new CFireball(fireballX, fireballY, vx, vy);
    scene->AddObject(fireball);
    hasShotFire = true;
}

