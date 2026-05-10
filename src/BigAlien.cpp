#include "BigAlien.hpp"
#include <raylib.h>

BigAlien::BigAlien(float x, float y) : Alien(x, y)
{
    health = 35;
}

void BigAlien::update()
{
    position.x += speedX;
}

void BigAlien::draw()
{
    float drawSize = WIDTH - 2;
    DrawRectangle(position.x - drawSize/2, position.y - drawSize/2, drawSize, drawSize, RED);
    float barWidth = drawSize * ((float)health / 35.0f);
    DrawRectangle(position.x - drawSize/2, position.y - drawSize/2 - 8, barWidth, 4, RED);
}