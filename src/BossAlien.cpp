#include "BossAlien.hpp"
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
    DrawRectangle(position.x - BOSS_DRAW_WIDTH/2, position.y - BOSS_DRAW_WIDTH/2,
                  BOSS_DRAW_WIDTH, BOSS_DRAW_WIDTH, PURPLE);
    float barWidth = BOSS_DRAW_WIDTH * ((float)health / 60.0f);
    DrawRectangle(position.x - BOSS_DRAW_WIDTH/2, position.y - BOSS_DRAW_WIDTH/2 - 10,
                  barWidth, 8, RED);
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