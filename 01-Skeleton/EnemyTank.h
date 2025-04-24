#pragma once
#include "Tank.h"

class CEnemyTank : public CTank {
private:
    float movedTimer = 1000; // Remaining time for the current movement
    int minMoveTime = 1000; // Minimum movement duration
    int maxMoveTime = 1500; // Maximum movement duration

public:
    CEnemyTank(float x, float y, LPTEXTURE texUp, LPTEXTURE texDown, LPTEXTURE texLeft, LPTEXTURE texRight, float speed, LPTEXTURE bulletTexture);
    ~CEnemyTank();

    void Update(DWORD dt) override;
    void HandleMove();
    void SetDirection(int dir);
    void Shoot();
};