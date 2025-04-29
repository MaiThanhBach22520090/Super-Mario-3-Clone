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

    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    if (isIdle)
    {
        if (GetTickCount64() - stateTimer >= PIRANHA_IDLE_TIME)
        {
            isIdle = false;
            if (isMovingUp)
                vy = -PIRANHA_MOVE_SPEED;
            else
                vy = PIRANHA_MOVE_SPEED;
        }
    }
    else
    {
        y += vy * dt;

        if (isMovingUp && y <= startY - PIRANHA_SPRITE_HEIGHT)
        {
            y = startY - PIRANHA_SPRITE_HEIGHT;
            isMovingUp = false;
            isIdle = true;
            vy = 0;
            stateTimer = GetTickCount64();

            if (isShootFire && !hasShotFire && mario != NULL)
            {
                float marioX, marioY;
                mario->GetPosition(marioX, marioY);

                float dx = marioX - x;
                float dy = marioY - y;
                float len = sqrtf(dx * dx + dy * dy);

                if (len == 0) len = 1.0f;

                float vx = 0.15f * dx / len;
                float vy = 0.15f * dy / len;

                float fireballX = x;
                float fireballY = y - PIRANHA_BBOX_HEIGHT / 2; // fire from center of hitbox

                CFireball* fireball = new CFireball(fireballX, fireballY, vx, vy);
                scene->AddObject(fireball);
                hasShotFire = true;
            }
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
}

void CPiranhaPlant::Render()
{
    CAnimations* animations = CAnimations::GetInstance();

    LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
    CMario* mario = (CMario*)scene->GetPlayer();

    if (mario != NULL)
    {
        float marioX, marioY;
        mario->GetPosition(marioX, marioY);

		if (marioX < x)
		{
			if (marioY < y)
			{
				animations->Get(ID_ANI_PIRANHA_UL)->Render(x, y);
			}
			else
			{
				animations->Get(ID_ANI_PIRANHA_DL)->Render(x, y);
			}
		}
		else
		{
			if (marioY < y)
			{
				animations->Get(ID_ANI_PIRANHA_UR)->Render(x, y);
			}
			else
			{
				animations->Get(ID_ANI_PIRANHA_DR)->Render(x, y);
			}
		}
	}

	RenderBoundingBox();
}

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - PIRANHA_BBOX_WIDTH / 2;
    t = y - PIRANHA_BBOX_HEIGHT;
    r = l + PIRANHA_BBOX_WIDTH;
    b = y;
}
