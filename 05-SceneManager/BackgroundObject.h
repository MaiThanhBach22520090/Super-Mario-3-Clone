#pragma once
#include "GameObject.h"

class CBackgroundObject : public CGameObject
{
protected:
    int animationId;

public:
    CBackgroundObject(float x, float y, int animationId) : CGameObject(x, y)
    {
        this->animationId = animationId;
    }

    void Render() override;

    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

    int IsBlocking() override { return 0; }

    int IsCollidable() override { return 0; }

    int IsDirectionColliable(float nx, float ny) override { return 0; }
};
