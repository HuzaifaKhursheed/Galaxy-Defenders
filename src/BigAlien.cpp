#include "BigAlien.hpp"
#include <raylib.h>

BigAlien::BigAlien(float x, float y) : Alien(x, y)
{
    health = 3;   // three hits to kill
}

void BigAlien::update()
{
    position.x += speedX;
}

void BigAlien::draw()
{
    DrawRectangle(position.x - WIDTH/2, position.y - WIDTH/2, WIDTH, WIDTH, RED);
}