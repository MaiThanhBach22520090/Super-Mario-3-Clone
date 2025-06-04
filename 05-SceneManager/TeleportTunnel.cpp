#include "TeleportTunnel.h"
#include "Textures.h"
#include "Game.h"
#include "Mario.h"

void CTeleportTunnel::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - width / 2;
	t = y - height / 2;
	r = l + width;
	b = t + height;
}

void CTeleportTunnel::Render()
{
	RenderBoundingBox();
}

void CTeleportTunnel::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Update teleporting timer
	teleportingTimer += dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CTeleportTunnel::RenderBoundingBox(void)
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;
	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	float xx = x - this->width / 2 + rect.right / 2;
	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CTeleportTunnel::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	if (!mario || mario->IsTeleporting()) return;
	mario->SetPosition(x, mario->GetY());
	{
		mario->StartTeleport(teleportX, teleportY, isGoingDown);
	}
}

void CTeleportTunnel::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (CMario * mario = dynamic_cast<CMario*>(e->obj))
	{
		OnCollisionWithMario(e);
	}
}

