#include "Alien.hpp"

Alien::Alien(float x, float y)
    : position{x, y}, health(1), speedX(SPEED_X)
{}

void Alien::takeDamage()
{
    --health;
}