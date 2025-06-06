#include "GUI.h"
#include "Game.h"
#include "Sprites.h"
#include "debug.h"

void CGUI::Render()
{
    float camX, camY;
    CGame::GetInstance()->GetCamPos(camX, camY);

    CSprites::GetInstance()->Get(MAIN_GUI_ID_SPRITE)->Draw(camX + 125, camY + 228);

    // Debug UI's Position
    DebugOut(L"[GUI] Cam Pos: %f, %f\n", camX, camY);

}

void CGUI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    // HUD doesn't move or update physics, but you can update displayed values here if needed
}

void CGUI::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = t = r = b = 0; // Not collidable
}
