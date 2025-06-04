#pragma once
#include "GameObject.h"

#define TELEPORT_TIME 400
#define TELEPORTING_SPEED 0.05f

class CTeleportTunnel : public CGameObject
{
protected:
    int width;
    int height;
    bool isGoingDown;
    float teleportTime;

    float teleportingTimer = 0;

    float teleportX;
    float teleportY;

public:
    CTeleportTunnel(float x, float y, int width, int height, bool isGoingDown, float teleportX, float teleportY)
        : CGameObject(x, y), width(width), height(height), isGoingDown(isGoingDown),
        teleportX(teleportX), teleportY(teleportY), teleportTime(TELEPORT_TIME)
    {
    }

    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;

    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
    void RenderBoundingBox(void);

    int IsBlocking() override { return 0; }
    int IsCollidable() override { return 1; }

    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void OnCollisionWithMario(LPCOLLISIONEVENT e);
};
