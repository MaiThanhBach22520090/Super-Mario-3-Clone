#include "Wing.h"
#include "Animations.h"

CWing::CWing(float offsetX, float offsetY, CGameObject* owner)
{
    this->offsetX = offsetX;
    this->offsetY = offsetY;
    this->owner = owner;
}

void CWing::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (owner)
    {
        x = owner->GetX() + offsetX;
        y = owner->GetY() + offsetY;
    }
}

void CWing::Render()
{
    CAnimations::GetInstance()->Get(ID_ANI_WING_FLAP)->Render(x, y);
}
