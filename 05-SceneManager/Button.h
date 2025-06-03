#pragma once
#include "GameObject.h"

#define BUTTON_BBOX_WIDTH     16
#define BUTTON_BBOX_HEIGHT    16

#define ID_ANI_BUTTON         10064
#define ID_ANI_BUTTON_PRESSED 10065

class CButton : public CGameObject
{
protected:
    bool pressed = false;

public:
    CButton(float x, float y);

    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();

    int IsCollidable() { return !pressed; }
    int IsBlocking() { return !pressed; }

    void OnCollisionWith(LPCOLLISIONEVENT e) override;
    void OnCollisionWithMario(LPCOLLISIONEVENT e);

    bool IsPressed() { return pressed; }
};
