#ifndef BULLET_HPP
#define BULLET_HPP

#include <raylib.h>

class Bullet {
public:
    Bullet(float startX, float startY, bool isPlayer);
    void update();
    void draw();
    bool isOutOfBounds() const;
    bool isPlayerBullet() const { return playerBullet; }
    Rectangle getBounds() const;

private:
    Vector2 position;
    float speedY;
    bool playerBullet;
    static constexpr float SIZE = 5.0f;
    static constexpr float PLAYER_SPEED = -9.0f;   // moderate upward speed (adjust to taste)
    static constexpr float ALIEN_SPEED = 3.0f;      // for later
};

#endif