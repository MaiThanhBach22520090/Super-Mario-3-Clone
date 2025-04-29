#include "Fireball.h"

CFireball::CFireball(float x, float y, float dirX, float dirY) : CGameObject(x, y)
{
    vx = dirX * FIREBALL_SPEED;
    vy = dirY * FIREBALL_SPEED;
}

void CFireball::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    animations->Get(ID_ANI_FIREBALL)->Render(x, y);

    //RenderBoundingBox(); // Optional debug
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    x += vx * dt;
    y += vy * dt;

    // Optional: delete if fireball goes offscreen
}

void CFireball::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - FIREBALL_WIDTH / 2;
    t = y - FIREBALL_HEIGHT / 2;
    r = l + FIREBALL_WIDTH;
    b = t + FIREBALL_HEIGHT;
}
