#include <raylib.h>


int main(){
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight , "Galaxy Defenders");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(400,300,50,50,RED);
        EndDrawing();


    }

    CloseWindow();
    return 0;


}
