#ifndef BOSSALIEN_HPP
#define BOSSALIEN_HPP

#include "Alien.hpp"

class BossAlien : public Alien {
public:
    BossAlien(float x, float y);
    void update() override;
    void draw() override;
    void takeDamage(int dmg) override;
    Rectangle getBounds() const override;
};

#endif