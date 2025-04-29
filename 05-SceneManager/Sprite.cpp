#include "Sprite.h"

#include "Game.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	float texWidth = (float)tex->getWidth();
	float texHeight = (float)tex->getHeight();

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	sprite.TexCoord.x = this->left / texWidth;
	sprite.TexCoord.y = this->top / texHeight;

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	sprite.TexSize.x = spriteWidth / texWidth;
	sprite.TexSize.y = spriteHeight / texHeight;

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);
}

void CSprite::Draw(float x, float y)
{
	CGame* g = CGame::GetInstance();
	float cx, cy;
	g->GetCamPos(cx, cy);

	cx = (FLOAT)floor(cx);
	cy = (FLOAT)floor(cy);

	D3DXMATRIX matTranslation;
	
	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	D3DXMatrixTranslation(&matTranslation, x - cx, g->GetBackBufferHeight() - y + cy, 0.1f);

	this->sprite.matWorld = (this->matScaling * matTranslation);

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

void CSprite::Draw(float x, float y, float scaleX, float scaleY)
{
	CGame* g = CGame::GetInstance();
	float cx, cy;
	g->GetCamPos(cx, cy);

	cx = (FLOAT)floor(cx);
	cy = (FLOAT)floor(cy);

	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	// Update texture info (should be done BEFORE draw!)
	sprite.pTexture = this->texture->getShaderResourceView();
	sprite.TexCoord.x = this->left / this->texture->getWidth();
	sprite.TexCoord.y = this->top / this->texture->getHeight();
	sprite.TexSize.x = (this->right - this->left + 1) / this->texture->getWidth();
	sprite.TexSize.y = (this->bottom - this->top + 1) / this->texture->getHeight();
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	float width = (float)(this->right - this->left + 1);
	float height = (float)(this->bottom - this->top + 1);

	// If flipping, adjust origin
	if (scaleX < 0) x += width;
	if (scaleY < 0) y += height;

	D3DXMATRIX matScaling, matTranslation;
	D3DXMatrixScaling(&matScaling, scaleX * width, scaleY * height, 1.0f);
	D3DXMatrixTranslation(&matTranslation, x - cx, g->GetBackBufferHeight() - y + cy, 0.1f);

	sprite.matWorld = matScaling * matTranslation;

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

