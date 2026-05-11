#include "BigAlien.hpp"
#include "ResourceManager.hpp"
#include <raylib.h>

BigAlien::BigAlien(float x, float y) : Alien(x, y)
{
    health = 35;
}

void BigAlien::update()
{
    position.x += speedX;
}

void BigAlien::draw()
{
    Texture2D tex = ResourceManager::getTexture("big_alien");
    Rectangle sourceRec = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    Rectangle destRec = { position.x, position.y, WIDTH, WIDTH };
    Vector2 origin = { WIDTH / 2.0f, WIDTH / 2.0f };
    DrawTexturePro(tex, sourceRec, destRec, origin, 0.0f, WHITE);
    float drawSize = WIDTH - 2; 
    float barWidth = drawSize * ((float)health / 35.0f);
    DrawRectangle(position.x - drawSize/2, position.y - drawSize/2 - 8, barWidth, 4, RED);
}