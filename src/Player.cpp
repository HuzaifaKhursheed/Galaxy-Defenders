#include "Player.hpp"

Player::Player()
{
    velocityX = 0.0f;
    position = { 400.0f, 550.0f };
    maxHealth = 70;
    health = maxHealth;
}

void Player::update()
{
    if (IsKeyDown(KEY_LEFT))
        velocityX -= ACC;
    if (IsKeyDown(KEY_RIGHT))
        velocityX += ACC;

    velocityX *= DRAG;
    if (velocityX < 0.05f && velocityX > -0.05f)
        velocityX = 0.0f;

    if (velocityX > MAX_SPEED)
        velocityX = MAX_SPEED;
    if (velocityX < -MAX_SPEED)
        velocityX = -MAX_SPEED;

    position.x += velocityX;
    if (position.x < WIDTH/2)
        position.x = WIDTH/2;
    if (position.x > 800 - WIDTH/2)
        position.x = 800 - WIDTH/2;
}

void Player::draw()
{
    DrawRectangle(position.x - WIDTH/2, position.y - WIDTH/2, WIDTH, WIDTH, BLUE);
}

Bullet Player::shoot() const
{
    float bulletX = position.x;
    float bulletY = position.y - WIDTH/2;
    return Bullet(bulletX, bulletY, velocityX, true, 1);
}

void Player::takeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

Rectangle Player::getBounds() const
{
    return { position.x - WIDTH/2, position.y - WIDTH/2, WIDTH, WIDTH };
}

void Player::setPosition(float x, float y)
{
    position = {x, y};
}

void Player::setHealth(int h)
{
    health = h;
    if (health > maxHealth) health = maxHealth;
    if (health < 0) health = 0;
}