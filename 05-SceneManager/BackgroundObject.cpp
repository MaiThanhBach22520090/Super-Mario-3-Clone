#include "BackgroundObject.h"
#include "Animations.h"

void CBackgroundObject::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    animations->Get(animationId)->Render(x, y);
}

void CBackgroundObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    // Return empty bounding box to ensure no collision logic depends on it
    l = t = r = b = 0;
}
