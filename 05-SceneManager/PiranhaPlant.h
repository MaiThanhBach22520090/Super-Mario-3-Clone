#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define PIRANHA_MOVE_SPEED 0.03f

#define PIRANHA_SPRITE_WIDTH 16
#define PIRANHA_SPRITE_HEIGHT 32

#define PIRANHA_BBOX_WIDTH 12
#define PIRANHA_BBOX_HEIGHT 24

#define PIRANHA_IDLE_TIME 1000

#define ID_ANI_PIRANHA 10031
#define ID_ANI_PIRANHA_DL 10032
#define ID_ANI_PIRANHA_UL 10033
#define ID_ANI_PIRANHA_DR 10034
#define ID_ANI_PIRANHA_UR 10035


class CPiranhaPlant : public CGameObject
{
protected:
    float startY;
    bool isMovingUp;
    bool isIdle;
    DWORD stateTimer;
	bool isShootFire;
    bool hasShotFire;

public:
    CPiranhaPlant(float x, float y);
	CPiranhaPlant(float x, float y, bool isShootFire);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    int IsBlocking() { return 0; }
    int IsCollidable()
    {
		return isMovingUp ? 0 : 1;
    }
};
