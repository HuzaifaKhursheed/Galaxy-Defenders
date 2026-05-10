#include "SmallAlien.hpp"
#include <raylib.h>

SmallAlien::SmallAlien(float x, float y) : Alien(x, y)
{
    health = 13;
}

void SmallAlien::update()
{
    position.x += speedX;
}

void SmallAlien::draw()
{
    
float drawSize = WIDTH - 2; // 38
DrawRectangle(position.x - drawSize/2, position.y - drawSize/2, drawSize, drawSize, GREEN);
float barWidth = drawSize * ((float)health / 13.0f);
DrawRectangle(position.x - drawSize/2, position.y - drawSize/2 - 8, barWidth, 4, RED);

}