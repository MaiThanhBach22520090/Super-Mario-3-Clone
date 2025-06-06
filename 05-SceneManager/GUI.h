#pragma once
#include "GameObject.h"

#define MAIN_GUI_ID_SPRITE 61025

#define ID_SPRITE_NUMBER_0   70000
#define ID_SPRITE_NUMBER_1   70001
#define ID_SPRITE_NUMBER_2   70002
#define ID_SPRITE_NUMBER_3   70003
#define ID_SPRITE_NUMBER_4   70004
#define ID_SPRITE_NUMBER_5   70005
#define ID_SPRITE_NUMBER_6   70006
#define ID_SPRITE_NUMBER_7   70007
#define ID_SPRITE_NUMBER_8   70008
#define ID_SPRITE_NUMBER_9   70009

#define TIME_LEFT 300000

class CGUI : public CGameObject
{
    DWORD timeLeft;
    DWORD lastUpdateTime;

public:
    CGUI(float x, float y) : CGameObject(x, y)
    {
        timeLeft = 300000; // e.g., 5 minutes (300,000 ms)
        lastUpdateTime = GetTickCount64();
    }

    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    void RenderNumber(int number, float x, float y, int maxDigits = 0);
    void StartTimer(DWORD totalMilliseconds);
};
