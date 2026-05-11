#include "Bullet.hpp"

Bullet::Bullet(float startX, float startY, float playerVx, bool isPlayer, int dmg)
{
    position.x = startX;
    position.y = startY;
    playerBullet = isPlayer;
    damage = dmg;
    if (playerBullet) {
        vx = playerVx;
        vy = PLAYER_SPEED_Y;
    } else {
        vx = 0.0f;
        if (damage == 1) vy = ALIEN_SMALL_SPEED;
        else if (damage == 5) vy = ALIEN_BIG_SPEED;
        else vy = ALIEN_BOSS_SPEED;
    }
}

void Bullet::update()
{
    if (playerBullet) vx *= BULLET_DRAG;
    position.x += vx;
    position.y += vy;
}

void Bullet::draw()
{
    Color color = playerBullet ? YELLOW : RED;
    float size = SIZE;
    if (!playerBullet && damage == 10) size = SIZE * 2.5f;   // boss bullet larger
    DrawRectangle(position.x - size/2, position.y - size/2, size, size, color);
}

bool Bullet::isOutOfBounds() const
{
    return (position.y + SIZE < 0) || (position.y > 600);
}

Rectangle Bullet::getBounds() const
{
    return { position.x - SIZE/2, position.y - SIZE/2, SIZE, SIZE };
}
