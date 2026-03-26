#include <raylib.h>
#include "Player.hpp"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Galaxy Defenders");
    SetTargetFPS(60);

    Player player;   // create the player object

    while (!WindowShouldClose())
    {
        player.update();        // update player (movement, etc.)

        BeginDrawing();
        ClearBackground(BLACK);
        player.draw();          // draw the player
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
