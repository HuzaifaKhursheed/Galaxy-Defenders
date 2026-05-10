#include "Alien.hpp"

Alien::Alien(float x, float y) : position{x, y}, health(1), speedX(SPEED_X) {}

void Alien::takeDamage(int dmg)
{
    health -= dmg;
    if (health < 0) health = 0;
}