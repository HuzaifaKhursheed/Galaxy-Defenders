#include "Player.hpp"

Player::Player()
{
    velocityX = 0.0f; //initially at rest
    position = { 400.0f, 550.0f };  //starting position,400 is half of window width so centered,550 is 50 pixels above the bottom(600)
}

void Player::update()
{
    //Check for key presses and change velocity
    if (IsKeyDown(KEY_LEFT))
        velocityX = velocityX - ACC;
    if (IsKeyDown(KEY_RIGHT))
        velocityX = velocityX + ACC;

    //Apply friction (slow down gradually)
    velocityX = velocityX * DRAG;

    //If speed is very small, set it to zero to avoid drifting
    if (velocityX < 0.05f && velocityX > -0.05f)
        velocityX = 0.0f;

    //Limiting maximum speed
    if (velocityX > MAX_SPEED)
        velocityX = MAX_SPEED;
    if (velocityX < -MAX_SPEED)
        velocityX = -MAX_SPEED;

    //Move the player by adding velocity to position
    position.x = position.x + velocityX;

    //To Keep the player inside the window edges
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
    return Bullet(bulletX, bulletY, true);
}