#pragma once
#include "GameObject.h"

#define ID_ANI_WING_FLAP 10050

class CWing : public CGameObject
{
protected:
    float offsetX, offsetY;
    CGameObject* owner;

public:
    CWing(float offsetX, float offsetY, CGameObject* owner);

    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
    void Render() override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override {}
};
