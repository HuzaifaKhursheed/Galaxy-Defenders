#include <raylib.h>
#include <vector>
#include <memory>          // for unique_ptr
#include <algorithm>       // for remove_if
#include "Player.hpp"
#include "Bullet.hpp"
#include "SmallAlien.hpp"
#include "BigAlien.hpp"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Galaxy Defenders");
    SetTargetFPS(60);

    Player player;
    std::vector<Bullet> bullets;
    std::vector<std::unique_ptr<Alien>> aliens;

    // Create aliens in a grid
    int rows = 2;
    int cols = 5;
    float startX = 100;
    float startY = 50;
    float spacing = 60;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            float x = startX + col * spacing;
            float y = startY + row * spacing;
            if (row == 0 && col % 2 == 0)
                aliens.push_back(std::make_unique<BigAlien>(x, y));
            else
                aliens.push_back(std::make_unique<SmallAlien>(x, y));
        }
    }

    // Group movement variables
    float groupSpeedX = 1.0f;
    float groupMoveDown = 20.0f;

    // Continuous fire system (unchanged)
    float continuousFireRemaining = 0.0f;
    const float maxFireDuration = 5.0f;
    const float shotInterval = 0.1f;
    float lastShotTime = 0.0f;

    // Power‑up placeholder (unchanged)
    bool powerupActive = false;
    float powerupTimer = 0.0f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // --- Input & continuous fire (unchanged) ---
        bool spaceHeld = IsKeyDown(KEY_SPACE);
        if (spaceHeld)
        {
            if (continuousFireRemaining > 0.0f)
            {
                float currentTime = GetTime();
                if (currentTime - lastShotTime >= shotInterval)
                {
                    bullets.push_back(player.shoot());
                    lastShotTime = currentTime;
                    continuousFireRemaining -= shotInterval;
                }
            }
        }
        else
        {
            continuousFireRemaining = maxFireDuration;
            lastShotTime = GetTime();
        }

        // Power‑up timer (unchanged)
        if (powerupActive)
        {
            powerupTimer -= dt;
            if (powerupTimer <= 0.0f)
            {
                powerupActive = false;
            }
        }

        // --- Update player ---
        player.update();

        // --- Update bullets (unchanged) ---
        for (size_t i = 0; i < bullets.size(); ++i)
            bullets[i].update();

        // --- Alien group movement ---
        // Check if any alien would hit the edge
        bool edgeReached = false;
        for (auto& alien : aliens)
        {
            float newX = alien->getX() + groupSpeedX;
            if (newX < 40 || newX > 760)
            {
                edgeReached = true;
                break;
            }
        }
        if (edgeReached)
        {
            groupSpeedX = -groupSpeedX;
            for (auto& alien : aliens)
            {
                float newY = alien->getY() + groupMoveDown;
                alien->setPosition(alien->getX(), newY);
            }
        }
        else
        {
            for (auto& alien : aliens)
            {
                float newX = alien->getX() + groupSpeedX;
                alien->setPosition(newX, alien->getY());
            }
        }

        // --- Collision: bullets vs aliens ---
        for (auto it = bullets.begin(); it != bullets.end(); )
        {
            bool hit = false;
            for (auto& alien : aliens)
            {
                if (alien->isAlive() && CheckCollisionRecs(it->getBounds(), alien->getBounds()))
                {
                    alien->takeDamage();
                    hit = true;
                    break;
                }
            }
            if (hit)
                it = bullets.erase(it);
            else
                ++it;
        }

        // Remove dead aliens
        aliens.erase(std::remove_if(aliens.begin(), aliens.end(),
            [](const std::unique_ptr<Alien>& a) { return !a->isAlive(); }),
            aliens.end());

        // Remove out-of-bounds bullets (unchanged)
        for (size_t i = 0; i < bullets.size(); )
        {
            if (bullets[i].isOutOfBounds())
                bullets.erase(bullets.begin() + i);
            else
                ++i;
        }

        // --- Draw ---
        BeginDrawing();
        ClearBackground(BLACK);
        player.draw();
        for (size_t i = 0; i < bullets.size(); ++i)
            bullets[i].draw();
        for (auto& alien : aliens)
            alien->draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}