#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <raylib.h>
#include "Bullet.hpp"

class Player {
public:
    Player();
    void update();
    void draw();
    Bullet shoot() const;
    void takeDamage(int damage);
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    void setHealth(int h);
    Rectangle getBounds() const;
    void setPosition(float x, float y);

private:
    Vector2 position;
    float velocityX;
    int health;
    int maxHealth;
    static constexpr float WIDTH = 40.0f;
    static constexpr float ACC = 0.5f;
    static constexpr float DRAG = 0.95f;
    static constexpr float MAX_SPEED = 5.0f;
};

#endif