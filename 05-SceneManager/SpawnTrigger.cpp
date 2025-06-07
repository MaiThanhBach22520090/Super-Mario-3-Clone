#include "SpawnTrigger.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopa.h"
#include "PlayScene.h"

void CSpawnTrigger::Render()
{
	//RenderBoundingBox();
}

void CSpawnTrigger::RenderBoundingBox(void)
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

void CSpawnTrigger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CSpawnTrigger::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking() && e->obj->IsCollidable())
	{
		if (dynamic_cast<CMario*>(e->obj))
		{
			OnCollisionWithMario(e);
		}
	}
}

void CSpawnTrigger::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	if (hasSpawned) return;
	CMario* mario = dynamic_cast<CMario*>(e->obj);
	if (!mario) return;
	// Spawn the enemy at the specified position
	CGameObject* enemy = nullptr;
	if (enemyType == 0) // Goomba
	{
		enemy = new CGoomba(spawnX, spawnY, spawnWithWings);
	}
	else if (enemyType == 1) // Koopa
	{
		enemy = new CKoopa(spawnX, spawnY, spawnWithWings);
	}
	if (enemy)
	{
		CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
		if (scene)
		{
			scene->AddObject(enemy);
			hasSpawned = true; // Mark as spawned
		}
	}
}

void CSpawnTrigger::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
    t = y;
    r = x + SPAWN_TRIGGER_BBOX_WIDTH * width;
    b = y + SPAWN_TRIGGER_BBOX_HEIGHT * height;
}
