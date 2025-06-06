#pragma once
#include "GameObject.h"

#define MAIN_GUI_ID_SPRITE 61025

class CGUI : public CGameObject
{
public:
    CGUI(float x, float y) : CGameObject(x, y) {};

    void Render() override;
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};
