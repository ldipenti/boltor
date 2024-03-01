#include "boltor/boltor.h"
#include "raylib.h"

int BoltorGame(int width, int height, int fps, char *title, void (*callback)(void))
{
    InitWindow(width, height, title);
    SetTargetFPS(fps);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        callback();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}