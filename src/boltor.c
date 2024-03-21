#include "boltor/boltor.h"
#include "raylib.h"

int BoltorGame(int width, int height, int fps, char *title, void (*callback)(void *), void *c)
{
    InitWindow(width, height, title);
    SetTargetFPS(fps);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        callback(c);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}