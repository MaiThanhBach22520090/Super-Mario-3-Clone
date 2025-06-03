#pragma once
#include "GameObject.h"

#define GOLDEN_BRICK_BBOX_WIDTH  16
#define GOLDEN_BRICK_BBOX_HEIGHT 16

#define ID_ANI_GOLDEN_BRICK      10063
#define ID_ANI_GOLDEN_BRICK_EMPTY 10021
#define ID_ANI_GOLDEN_COIN       11000

class CGoldenBrick : public CGameObject
{
protected:
    bool destroyed = false;
    bool isCoin = false;
    bool containsButton = false;
    bool buttonRevealed = false;

public:
    CGoldenBrick(float x, float y, bool hasButton);

    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();

    int IsCollidable() { return 1; }
    int IsBlocking() { return 1; }

	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	// Collision handlers
    void OnCollisionWithTail(LPCOLLISIONEVENT e);
	void OnCollisionWithShell(LPCOLLISIONEVENT e);

    void OnButtonActivated(); // Called by the Button
};
