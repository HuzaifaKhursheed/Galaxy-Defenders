#ifndef BIGALIEN_HPP
#define BIGALIEN_HPP

#include "Alien.hpp"

class BigAlien : public Alien {
public:
    BigAlien(float x, float y);
    void update() override;
    void draw() override;
};

#endif