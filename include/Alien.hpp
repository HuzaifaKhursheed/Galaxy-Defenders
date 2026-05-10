#ifndef ALIEN_HPP
#define ALIEN_HPP

#include <raylib.h>

class Alien {
public:
    Alien(float x, float y);
    virtual ~Alien() = default;

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void takeDamage(int dmg);

    bool isAlive() const { return health > 0; }
    float getX() const { return position.x; }
    float getY() const { return position.y; }
    void setPosition(float x, float y) { position = {x, y}; }
    virtual Rectangle getBounds() const { return { position.x - 20, position.y - 20, 40, 40 }; }
    int getHealth() const { return health; }

protected:
    Vector2 position;
    int health;
    float speedX;
    static constexpr float WIDTH = 40.0f;
    static constexpr float SPEED_X = 1.0f;
};

#endif