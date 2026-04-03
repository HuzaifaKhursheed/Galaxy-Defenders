#include "Bullet.hpp"

Bullet::Bullet(float startX, float startY, bool isPlayer)
{
    position.x = startX;
    position.y = startY;
    playerBullet = isPlayer;
    if (playerBullet)
        speedY = PLAYER_SPEED;
    else
        speedY = ALIEN_SPEED;
}

Rectangle Bullet::getBounds() const
{
    return { position.x - SIZE/2, position.y - SIZE/2, SIZE, SIZE };
}


void Bullet::update()
{
    position.y += speedY;
}

void Bullet::draw()
{
    DrawRectangle(position.x - SIZE/2, position.y - SIZE/2, SIZE, SIZE, YELLOW);
}

bool Bullet::isOutOfBounds() const
{
    return (position.y + SIZE < 0) || (position.y > 600);
}
