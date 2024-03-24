#include "boltor/boltor.h"
#include "raylib.h"

int BTRGame(int width, int height, int fps, char *title, BTRGameData g)
{
    InitWindow(width, height, title);
    SetTargetFPS(fps);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        g.gameLoop(g.data);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
