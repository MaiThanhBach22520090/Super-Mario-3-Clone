#pragma once
#include "GameObject.h"

class CBullet : public CGameObject
{
private:
    float speed; // Speed of the bullet
    bool active; // Whether the bullet is active (on screen)
public:
    CBullet(float x, float y, LPTEXTURE texture, float speed = 0.5f);

    void Update(DWORD dt) override; // Update the bullet's position
    void Render() override;        // Render the bullet if it's active

    bool IsActive() const { return active; } // Check if the bullet is active
    void SetActive(bool active) { this->active = active; } // Set the bullet's active state

    void SetVelocity(float vx, float vy) { this->vx = vx; this->vy = vy; } // Set the bullet's velocity
};

