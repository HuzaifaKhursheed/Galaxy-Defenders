#include "BossAlien.hpp"
#include "ResourceManager.hpp"
#include <raylib.h>

BossAlien::BossAlien(float x, float y) : Alien(x, y)
{
    health = 60;
    speedX = 0.5f;
}

void BossAlien::update()
{
    position.x += speedX;
    if (position.x < 20) { position.x = 20; speedX = -speedX; }
    if (position.x > 780) { position.x = 780; speedX = -speedX; }
}

void BossAlien::draw()
{
    const float BOSS_DRAW_WIDTH = 120.0f;
    Texture2D tex = ResourceManager::getTexture("boss_alien");
    
    Rectangle sourceRec = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    Rectangle destRec = { position.x, position.y, BOSS_DRAW_WIDTH, BOSS_DRAW_WIDTH };
    Vector2 origin = { BOSS_DRAW_WIDTH / 2.0f, BOSS_DRAW_WIDTH / 2.0f };

    DrawTexturePro(tex, sourceRec, destRec, origin, 0.0f, WHITE);

    // Health bar logic
    float barWidth = BOSS_DRAW_WIDTH * ((float)health / 60.0f);
    DrawRectangle(position.x - BOSS_DRAW_WIDTH/2, position.y - BOSS_DRAW_WIDTH/2 - 10, barWidth, 8, RED);
}

void BossAlien::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;
}

Rectangle BossAlien::getBounds() const
{
    return { position.x - 20, position.y - 20, 40, 40 };
}