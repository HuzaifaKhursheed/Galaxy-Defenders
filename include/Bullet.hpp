#ifndef BULLET_HPP
#define BULLET_HPP

#include <raylib.h>

class Bullet {
public:
    Bullet(float startX, float startY, float playerVx, bool isPlayer, int dmg = 1);
    void update();
    void draw();
    bool isOutOfBounds() const;
    bool isPlayerBullet() const { return playerBullet; }
    int getDamage() const { return damage; }
    Rectangle getBounds() const;

private:
    Vector2 position;
    float vx, vy;
    bool playerBullet;
    int damage;
    static constexpr float SIZE = 6.0f;                  // slightly larger for visibility
    static constexpr float PLAYER_SPEED_Y = -3.2f;       // slower – immediate curve
    static constexpr float ALIEN_SMALL_SPEED = 3.0f;
    static constexpr float ALIEN_BIG_SPEED = 4.0f;
    static constexpr float ALIEN_BOSS_SPEED = 5.0f;
    static constexpr float BULLET_DRAG = 0.995f;         // very low drag – curve lasts long
};

#endif