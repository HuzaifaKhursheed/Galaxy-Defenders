#include "SmallAlien.hpp"
#include <raylib.h>

SmallAlien::SmallAlien(float x, float y) : Alien(x, y)
{
    health = 1;   // one hit kill
}

void SmallAlien::update()
{
    // simple horizontal movement (group direction will be handled by the manager)
    position.x += speedX;
}

void SmallAlien::draw()
{
    DrawRectangle(position.x - WIDTH/2, position.y - WIDTH/2, WIDTH, WIDTH, GREEN);
}