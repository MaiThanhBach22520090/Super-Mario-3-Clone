#pragma once
#include "GameObject.h"
#include "debug.h"

#define TILE_SIZE 16

#define ID_SPRITE_END_LEVEL_EDGE 61026
#define ID_SPRITE_END_LEVEL_ZONE 61027


class CEndLevelZone : public CGameObject
{
protected:
	float width;
	float height;
	virtual int IsCollidable() { return 0; }
	virtual int IsBlocking() { return 0; } 

public:
	CEndLevelZone(float x, float y, int width, int height) : CGameObject(x, y)
	{
        this->x = x;
        this->y = y;
		this->width = 50;
		this->height = 50;
	}

	void GetBoundingBox(float& left, float& top, float& right, float& bottom) override
	{
		left = top = right = bottom = 0; // No bounding box for end level zone
	}

    void Render() override
    {
        int rows = 50;
        int cols = 50;

		DebugOut(L"[INFO] Rendering End Level Zone at (%f, %f) with size (%d, %d)\n", x, y, 50, 50);

        // Render left edge
        for (int row = 0; row < rows; row++)
        {
            CSprites::GetInstance()->Get(ID_SPRITE_END_LEVEL_EDGE)->Draw(x, y + row * TILE_SIZE);
        }

        // Render the rest of the zone
        for (int col = 1; col < cols; col++)
        {
            for (int row = 0; row < rows; row++)
            {
                CSprites::GetInstance()->Get(ID_SPRITE_END_LEVEL_ZONE)->Draw(x + col * TILE_SIZE, y + row * TILE_SIZE);
            }
        }
    }
};

