#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define FIREBALL_SPEED 0.2f
#define FIREBALL_WIDTH 8
#define FIREBALL_HEIGHT 8
#define ID_ANI_FIREBALL 10036

class CFireball : public CGameObject
{
protected:
    float vx, vy;
public:
    CFireball(float x, float y, float dirX, float dirY);

    virtual void Render();
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    int IsBlocking() { return 0; } // Fireball does not block others
};
