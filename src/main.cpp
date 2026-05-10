#include <raylib.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include "Player.hpp"
#include "Bullet.hpp"
#include "SmallAlien.hpp"
#include "BigAlien.hpp"
#include "BossAlien.hpp"

enum GameState { START_MENU, PLAYING, GAME_OVER };

template<typename T>
std::vector<T*> getRandomAliveAliens(const std::vector<std::unique_ptr<Alien>>& aliens, int count) {
    std::vector<T*> alive;
    for (auto& a : aliens) if (dynamic_cast<T*>(a.get()) && a->isAlive()) alive.push_back(static_cast<T*>(a.get()));
    std::shuffle(alive.begin(), alive.end(), std::default_random_engine(GetRandomValue(0, 100000)));
    if ((int)alive.size() > count) alive.resize(count);
    return alive;
}

void spawnWave(int waveNum, std::vector<std::unique_ptr<Alien>>& aliens) {
    aliens.clear();
    if (waveNum == 1) {
        for (int row = 0; row < 2; ++row)
            for (int i = 0; i < 10; ++i)
                aliens.push_back(std::make_unique<SmallAlien>(80 + i * 70, 80 + row * 60));
    } else if (waveNum == 2) {
        for (int row = 0; row < 2; ++row) {
            float y = 200 + row * 60;
            for (int i = 0; i < 10; ++i) {
                float x = (i < 5) ? 100 + i * 45 : 550 + (i-5) * 45;
                aliens.push_back(std::make_unique<SmallAlien>(x, y));
            }
        }
        for (int row = 0; row < 2; ++row)
            for (int i = 0; i < 5; ++i)
                aliens.push_back(std::make_unique<BigAlien>(150 + i * 120, 80 + row * 60));
    } else if (waveNum == 3) {
        aliens.push_back(std::make_unique<BossAlien>(400, 80));
        for (float x : {300, 400, 500}) aliens.push_back(std::make_unique<BigAlien>(x, 180));
        for (float x : {150, 200, 600, 650}) aliens.push_back(std::make_unique<BigAlien>(x, 180));
        for (int row = 0; row < 2; ++row) {
            float y = 260 + row * 60;
            for (int i = 0; i < 10; ++i) {
                float x = (i < 5) ? 100 + i * 45 : 550 + (i-5) * 45;
                aliens.push_back(std::make_unique<SmallAlien>(x, y));
            }
        }
    }
}

int main() {
    InitWindow(800, 600, "Galaxy Defenders");
    SetTargetFPS(60);

    // Load start menu image (optional)
    Texture2D startMenuTexture = LoadTexture("assets/start menu.jpeg");
    bool imageLoaded = startMenuTexture.id != 0;

    // Load background image for gameplay
    Texture2D bgTexture = LoadTexture("assets/backgroundFinal.png");
    // If background fails to load, we'll just keep black background (no crash)

    GameState state = START_MENU;
    Rectangle startButton = { 400 - 40, 380, 80, 40 }; // will be recalculated if needed

    Player player;
    std::vector<Bullet> bullets;
    std::vector<std::unique_ptr<Alien>> aliens;
    int currentWave = 1;
    float waveEndTimer = 0.0f;
    bool waveActive = true;
    float groupSpeedX = 1.0f;
    const float groupMoveDown = 20.0f;
    int edgeHitCountWave1 = 0, edgeHitCountWave2 = 0, edgeHitCountWave3 = 0;
    int wave2DescentCount = 0;
    int wave3VerticalDir = 1, wave3DescentCount = 0;
    const int WAVE3_STEPS_PER_DIR = 3;

    float continuousFireRemaining = 0.0f;
    const float maxFireDuration = 5.0f, shotInterval = 0.04f;
    float lastShotTime = 0.0f;

    float smallShootTimer = 0.0f, bigShootTimer = 0.0f;
    float smallWave2ShootTimer = 0.0f, smallWave3ShootTimer = 0.0f;
    float bossShootTimer = 0.0f, minionShootTimer = 0.0f, sideBigShootTimer = 0.0f;
    const float SMALL_BURST_INTERVAL = 1.5f;
    const float BIG_BURST_INTERVAL_WAVE2 = 1.6f;
    const float MINION_BURST_INTERVAL = 1.8f;
    const float SIDE_BIG_INTERVAL = 2.3f;
    const float BOSS_SHOOT_INTERVAL = 3.5f;

    std::random_device rd;
    std::mt19937 rng(rd());

    bool playerWonFlag = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (state == START_MENU) {
            if (!imageLoaded) {
                const char* startText = "START";
                int fontSize = 30;
                int tw = MeasureText(startText, fontSize);
                startButton.x = 400 - tw/2;
                startButton.y = 380;
                startButton.width = tw;
                startButton.height = fontSize;
            } else {
                // For image, use predefined button coordinates (adjust as needed)
                startButton = { 330, 380, 140, 50 };
            }

            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                state = PLAYING;
                // Reset game
                player = Player();
                bullets.clear();
                aliens.clear();
                currentWave = 1;
                waveActive = true;
                waveEndTimer = 0.0f;
                groupSpeedX = 1.0f;
                edgeHitCountWave1 = edgeHitCountWave2 = edgeHitCountWave3 = 0;
                wave2DescentCount = 0;
                wave3VerticalDir = 1;
                wave3DescentCount = 0;
                continuousFireRemaining = 0.0f;
                lastShotTime = 0.0f;
                smallShootTimer = bigShootTimer = smallWave2ShootTimer = smallWave3ShootTimer = 0.0f;
                bossShootTimer = minionShootTimer = sideBigShootTimer = 0.0f;
                spawnWave(currentWave, aliens);
            }
        }
        else if (state == PLAYING) {
            // Player shooting
            bool spaceHeld = IsKeyDown(KEY_SPACE);
            if (spaceHeld && continuousFireRemaining > 0.0f) {
                float cur = GetTime();
                if (cur - lastShotTime >= shotInterval) {
                    bullets.push_back(player.shoot());
                    lastShotTime = cur;
                    continuousFireRemaining -= shotInterval;
                }
            } else if (!spaceHeld) {
                continuousFireRemaining = maxFireDuration;
                lastShotTime = GetTime();
            }

            player.update();
            for (auto& b : bullets) b.update();

            // Alien group movement
            if (!aliens.empty()) {
                bool edge = false;
                for (auto& a : aliens) if (a->getX() + groupSpeedX < 40 || a->getX() + groupSpeedX > 760) { edge = true; break; }
                if (edge) {
                    groupSpeedX = -groupSpeedX;
                    if (currentWave == 1) {
                        if (++edgeHitCountWave1 >= 2) { for (auto& a : aliens) a->setPosition(a->getX(), a->getY() + groupMoveDown); edgeHitCountWave1 = 0; }
                    } else if (currentWave == 2) {
                        if (wave2DescentCount < 3 && ++edgeHitCountWave2 >= 3) { for (auto& a : aliens) a->setPosition(a->getX(), a->getY() + groupMoveDown); wave2DescentCount++; edgeHitCountWave2 = 0; }
                    } else if (currentWave == 3) {
                        if (++edgeHitCountWave3 >= 2) { float dy = wave3VerticalDir * groupMoveDown; for (auto& a : aliens) a->setPosition(a->getX(), a->getY() + dy); wave3DescentCount++; edgeHitCountWave3 = 0; if (wave3DescentCount >= WAVE3_STEPS_PER_DIR) { wave3VerticalDir = -wave3VerticalDir; wave3DescentCount = 0; } }
                    }
                } else { for (auto& a : aliens) a->setPosition(a->getX() + groupSpeedX, a->getY()); }
            }

            // Alien shooting (full logic)
            if (currentWave == 1) {
                smallShootTimer += dt;
                if (smallShootTimer >= SMALL_BURST_INTERVAL) {
                    smallShootTimer = 0.0f;
                    int alive = 0; for (auto& a : aliens) if (dynamic_cast<SmallAlien*>(a.get()) && a->isAlive()) alive++;
                    int cnt = (alive >= 8) ? 4 : (alive >= 5) ? 3 : (alive >= 3) ? 2 : (alive >= 1) ? 1 : 0;
                    auto shooters = getRandomAliveAliens<SmallAlien>(aliens, cnt);
                    for (auto* s : shooters) bullets.push_back(Bullet(s->getX(), s->getY() + 20, 0.0f, false, 1));
                }
            } else if (currentWave == 2) {
                bigShootTimer += dt;
                if (bigShootTimer >= BIG_BURST_INTERVAL_WAVE2) {
                    bigShootTimer = 0.0f;
                    int alive = 0; for (auto& a : aliens) if (dynamic_cast<BigAlien*>(a.get()) && a->isAlive()) alive++;
                    int cnt = (alive > 7) ? 5 : (alive >= 3) ? 3 : (alive == 2) ? 2 : (alive == 1) ? 1 : 0;
                    auto shooters = getRandomAliveAliens<BigAlien>(aliens, cnt);
                    for (auto* s : shooters) bullets.push_back(Bullet(s->getX(), s->getY() + 20, 0.0f, false, 5));
                }
                smallWave2ShootTimer += dt;
                if (smallWave2ShootTimer >= SMALL_BURST_INTERVAL) {
                    smallWave2ShootTimer = 0.0f;
                    int alive = 0; for (auto& a : aliens) if (dynamic_cast<SmallAlien*>(a.get()) && a->isAlive()) alive++;
                    int cnt = (alive > 10) ? 7 : (alive > 5) ? 4 : (alive >= 1) ? 3 : 0;
                    auto shooters = getRandomAliveAliens<SmallAlien>(aliens, cnt);
                    for (auto* s : shooters) bullets.push_back(Bullet(s->getX(), s->getY() + 20, 0.0f, false, 1));
                }
            } else if (currentWave == 3) {
                bossShootTimer += dt;
                if (bossShootTimer >= BOSS_SHOOT_INTERVAL) {
                    bossShootTimer = 0.0f;
                    for (auto& a : aliens) if (dynamic_cast<BossAlien*>(a.get()) && a->isAlive()) {
                        bullets.push_back(Bullet(a->getX(), a->getY() + 60, 0.0f, false, 10));
                        break;
                    }
                }
                minionShootTimer += dt;
                if (minionShootTimer >= MINION_BURST_INTERVAL) {
                    minionShootTimer = 0.0f;
                    std::vector<BigAlien*> minions;
                    for (auto& a : aliens) if (dynamic_cast<BigAlien*>(a.get()) && a->isAlive() && a->getX() > 280 && a->getX() < 520)
                        minions.push_back(static_cast<BigAlien*>(a.get()));
                    int cnt = (minions.size() >= 2) ? 2 : minions.size();
                    std::shuffle(minions.begin(), minions.end(), rng);
                    for (int i = 0; i < cnt; ++i) bullets.push_back(Bullet(minions[i]->getX(), minions[i]->getY() + 20, 0.0f, false, 5));
                }
                sideBigShootTimer += dt;
                if (sideBigShootTimer >= SIDE_BIG_INTERVAL) {
                    sideBigShootTimer = 0.0f;
                    std::vector<BigAlien*> left, right;
                    for (auto& a : aliens) if (dynamic_cast<BigAlien*>(a.get()) && a->isAlive()) {
                        if (a->getX() < 250) left.push_back(static_cast<BigAlien*>(a.get()));
                        else if (a->getX() > 550) right.push_back(static_cast<BigAlien*>(a.get()));
                    }
                    if (!left.empty() && !right.empty()) {
                        std::shuffle(left.begin(), left.end(), rng);
                        std::shuffle(right.begin(), right.end(), rng);
                        bullets.push_back(Bullet(left[0]->getX(), left[0]->getY() + 20, 0.0f, false, 5));
                        bullets.push_back(Bullet(right[0]->getX(), right[0]->getY() + 20, 0.0f, false, 5));
                    } else if (!left.empty()) {
                        std::shuffle(left.begin(), left.end(), rng);
                        bullets.push_back(Bullet(left[0]->getX(), left[0]->getY() + 20, 0.0f, false, 5));
                    } else if (!right.empty()) {
                        std::shuffle(right.begin(), right.end(), rng);
                        bullets.push_back(Bullet(right[0]->getX(), right[0]->getY() + 20, 0.0f, false, 5));
                    }
                }
                smallWave3ShootTimer += dt;
                if (smallWave3ShootTimer >= SMALL_BURST_INTERVAL) {
                    smallWave3ShootTimer = 0.0f;
                    int alive = 0; for (auto& a : aliens) if (dynamic_cast<SmallAlien*>(a.get()) && a->isAlive()) alive++;
                    int cnt = (alive > 10) ? 7 : (alive > 5) ? 4 : (alive >= 1) ? 3 : 0;
                    auto shooters = getRandomAliveAliens<SmallAlien>(aliens, cnt);
                    for (auto* s : shooters) bullets.push_back(Bullet(s->getX(), s->getY() + 20, 0.0f, false, 1));
                }
            }

            // Collision detection
            for (auto it = bullets.begin(); it != bullets.end(); ) {
                bool hit = false;
                if (it->isPlayerBullet()) {
                    for (auto& a : aliens) if (a->isAlive() && CheckCollisionRecs(it->getBounds(), a->getBounds())) { a->takeDamage(it->getDamage()); hit = true; break; }
                } else {
                    if (CheckCollisionRecs(it->getBounds(), player.getBounds())) { player.takeDamage(it->getDamage()); hit = true; }
                }
                if (hit) it = bullets.erase(it);
                else ++it;
            }

            aliens.erase(std::remove_if(aliens.begin(), aliens.end(), [](const auto& a) { return !a->isAlive(); }), aliens.end());
            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return b.isOutOfBounds(); }), bullets.end());

            if (player.getHealth() <= 0) { state = GAME_OVER; playerWonFlag = false; }
            else if (aliens.empty() && currentWave == 3) { state = GAME_OVER; playerWonFlag = true; }

            // Wave progression
            if (aliens.empty() && waveActive) {
                waveEndTimer += dt;
                if (waveEndTimer >= 1.5f) {
                    currentWave++;
                    if (currentWave <= 3) {
                        spawnWave(currentWave, aliens);
                        groupSpeedX = 1.0f;
                        waveEndTimer = 0.0f;
                        edgeHitCountWave1 = edgeHitCountWave2 = edgeHitCountWave3 = 0;
                        wave2DescentCount = 0;
                        wave3VerticalDir = 1; wave3DescentCount = 0;
                        smallShootTimer = bigShootTimer = smallWave2ShootTimer = smallWave3ShootTimer = 0.0f;
                        bossShootTimer = minionShootTimer = sideBigShootTimer = 0.0f;
                    } else waveActive = false;
                }
            } else if (!aliens.empty()) waveEndTimer = 0.0f;
        }
        else if (state == GAME_OVER) {
            if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) state = START_MENU;
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        if (state == START_MENU) {
            if (imageLoaded) {
                DrawTexture(startMenuTexture, 0, 0, WHITE);
            } else {
                DrawText("GALAXY DEFENDERS", 400 - MeasureText("GALAXY DEFENDERS", 40)/2, 200, 40, YELLOW);
                DrawText("START", startButton.x, startButton.y, 30, WHITE);
                DrawRectangle(startButton.x - 10, startButton.y - 5, startButton.width + 20, startButton.height + 10, Fade(BLUE, 0.3f));
            }
        }
        else if (state == PLAYING) {
            // Draw background image (if loaded, otherwise black)
            if (bgTexture.id != 0) DrawTexture(bgTexture, 0, 0, WHITE);
            else ClearBackground(BLACK); // already cleared, but just in case

            // Health bar
            float hpPercent = (float)player.getHealth() / player.getMaxHealth();
            DrawRectangle(10, 10, 200, 20, RED);
            DrawRectangle(10, 10, 200 * hpPercent, 20, GREEN);
            DrawText(TextFormat("Health: %d/%d", player.getHealth(), player.getMaxHealth()), 10, 35, 20, WHITE);

            // Info box
            DrawRectangle(10, 70, 200, 60, {0,0,255,180});
            DrawRectangleLines(10, 70, 200, 60, BLUE);
            DrawCircle(30, 90, 12, BLUE);
            DrawText("i", 26, 85, 18, WHITE);
            if (currentWave == 1) {
                DrawText("Per hit:", 55, 85, 14, WHITE);
                DrawText("Small alien: 1 dmg", 55, 105, 12, WHITE);
            } else if (currentWave == 2) {
                DrawText("Per hit:", 55, 80, 14, WHITE);
                DrawText("Big alien: 5 dmg", 55, 98, 12, WHITE);
                DrawText("Small alien: 1 dmg", 55, 114, 12, WHITE);
            } else {
                DrawText("Per hit:", 55, 80, 14, WHITE);
                DrawText("Boss: 10 dmg", 55, 98, 12, WHITE);
                DrawText("Big alien: 5 dmg", 55, 114, 12, WHITE);
            }

            player.draw();
            for (auto& b : bullets) b.draw();
            for (auto& a : aliens) a->draw();
        }
        else if (state == GAME_OVER) {
            const char* msg = playerWonFlag ? "YOU WIN!" : "YOU LOSE!";
            Color col = playerWonFlag ? GREEN : RED;
            DrawText(msg, 400 - MeasureText(msg, 60)/2, 270, 60, col);
            DrawText("Press SPACE or ENTER", 400 - MeasureText("Press SPACE or ENTER", 20)/2, 400, 20, WHITE);
        }

        EndDrawing();
    }

    UnloadTexture(startMenuTexture);
    if (bgTexture.id != 0) UnloadTexture(bgTexture);
    CloseWindow();
    return 0;
}