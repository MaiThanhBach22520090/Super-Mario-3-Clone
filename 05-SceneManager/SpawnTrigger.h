#pragma once
#include "GameObject.h"

#define SPAWN_TRIGGER_BBOX_WIDTH 16
#define SPAWN_TRIGGER_BBOX_HEIGHT 16

class CSpawnTrigger : public CGameObject
{
    int width;
    int height;

    int spawnX;
	int spawnY;

    bool hasSpawned = false;
    int enemyType;
    bool spawnWithWings;

public:
	CSpawnTrigger(float x, float y, int width, int height, int spawnX, int spawnY,  int enemyType, bool spawnWithWings)
		: CGameObject(x, y), width(width), height(height), spawnX(spawnX), spawnY(spawnY), enemyType(enemyType), spawnWithWings(spawnWithWings)
	{
		hasSpawned = false; // Initially not spawned
	}

    void Render() override;
	void RenderBoundingBox(void);
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
    void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnCollisionWithMario(LPCOLLISIONEVENT e);

    int IsCollidable() override { return !hasSpawned; }
    int IsBlocking() override { return 0; }
};
