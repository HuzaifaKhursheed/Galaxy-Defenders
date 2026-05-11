#include "SmallAlien.hpp"
#include "ResourceManager.hpp"
#include <raylib.h>

SmallAlien::SmallAlien(float x, float y) : Alien(x, y)
{
    health = 13;
}

void SmallAlien::update()
{
    position.x += speedX;
}

void SmallAlien::draw()
{
    Texture2D tex = ResourceManager::getTexture("small_alien");
    
    Rectangle sourceRec = { 0.0f, 0.0f, (float)tex.width, (float)tex.height };
    Rectangle destRec = { position.x, position.y, WIDTH, WIDTH };
    Vector2 origin = { WIDTH / 2.0f, WIDTH / 2.0f };

    DrawTexturePro(tex, sourceRec, destRec, origin, 0.0f, WHITE);

    // Keep your health bar logic exactly as it is!
    float drawSize = WIDTH - 2; 
    float barWidth = drawSize * ((float)health / 13.0f);
    DrawRectangle(position.x - drawSize/2, position.y - drawSize/2 - 8, barWidth, 4, RED);
}