#ifndef SMALLALIEN_HPP
#define SMALLALIEN_HPP

#include "Alien.hpp"

class SmallAlien : public Alien {
public:
    SmallAlien(float x, float y);
    void update() override;
    void draw() override;
};

#endif