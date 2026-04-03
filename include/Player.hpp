#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <raylib.h>
#include "Bullet.hpp"


class Player{
    public:
    Player();
    void update();  //called every frame to update movement
    void draw();   //called every frame to draw the player
    Bullet shoot() const;
    float getVelocityX() const { return velocityX; }

    private:
    Vector2 position;   //player position(x,y)
    float velocityX;   // horizontal speed,+ve means right,-ve means left
    static constexpr float WIDTH = 40.0f;
    static constexpr float ACC = 0.5f;
    static constexpr float DRAG = 0.95f;
    static constexpr float MAX_SPEED = 5.0f;

};

#endif