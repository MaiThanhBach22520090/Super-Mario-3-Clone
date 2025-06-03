#include "Button.h"
#include "GoldenBrick.h"
#include "PlayScene.h"
#include "Game.h"
#include "Mario.h"

CButton::CButton(float x, float y) : CGameObject(x, y)
{
}

void CButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - BUTTON_BBOX_WIDTH / 2;
    t = y - BUTTON_BBOX_HEIGHT / 2;
    r = l + BUTTON_BBOX_WIDTH;
    b = t + BUTTON_BBOX_HEIGHT;
}

void CButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CButton::Render()
{
    int aniId = pressed ? ID_ANI_BUTTON_PRESSED : ID_ANI_BUTTON;
    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CButton::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
	{
		OnCollisionWithMario(e);
	}
}

void CButton::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	if (pressed) return;
	if (e->ny <= 0) return; // Mario must be above the button
	pressed = true;
	// Transform all golden bricks
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> objects = scene->GetGameObjects();
	for (LPGAMEOBJECT obj : objects)
	{
		CGoldenBrick* brick = dynamic_cast<CGoldenBrick*>(obj);
		if (brick)
			brick->OnButtonActivated();
	}
}

//void CButton::OnMarioTouch()
//{
//    if (pressed) return;
//    pressed = true;
//
//    // Transform all golden bricks
//    CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
//    vector<LPGAMEOBJECT> objects = scene->GetGameObjects();
//
//    for (LPGAMEOBJECT obj : objects)
//    {
//        CGoldenBrick* brick = dynamic_cast<CGoldenBrick*>(obj);
//        if (brick)
//            brick->OnButtonActivated();
//    }
//}
