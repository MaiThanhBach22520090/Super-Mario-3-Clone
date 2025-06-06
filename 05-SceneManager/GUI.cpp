#include "GUI.h"
#include "Game.h"
#include "Sprites.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"


void CGUI::Render()
{
    float camX, camY;
    CGame::GetInstance()->GetCamPos(camX, camY);

    CSprites::GetInstance()->Get(MAIN_GUI_ID_SPRITE)->Draw(camX + 125, camY + 228);

	// Get Mario's Coin count or other values to display
    CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int coinCount = mario->GetCoinCount(); // Assuming GetCoinCount() returns the number of coins Mario has

    RenderNumber(coinCount, camX + 65, camY + 224, 7);
}

void CGUI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    // HUD doesn't move or update physics, but you can update displayed values here if needed
}

void CGUI::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = t = r = b = 0; // Not collidable
}

void CGUI::RenderNumber(int number, float x, float y, int maxDigits)
{
    CSprites* sprites = CSprites::GetInstance();

    std::string numStr = std::to_string(number);

    // Pad with zeros if needed
    if (maxDigits > 0 && numStr.length() < (size_t)maxDigits)
        numStr = std::string(maxDigits - numStr.length(), '0') + numStr;

    for (size_t i = 0; i < numStr.length(); ++i)
    {
        int digit = numStr[i] - '0';
        int spriteID = ID_SPRITE_NUMBER_0 + digit;

        LPSPRITE digitSprite = sprites->Get(spriteID);
        if (digitSprite != nullptr)
        {
            digitSprite->Draw(x + i * 8, y);
        }
        else
        {
            DebugOut(L"[ERROR] Sprite for digit %d (ID: %d) not found!\n", digit, spriteID);
        }

		
    }

}
