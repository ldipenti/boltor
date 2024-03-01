#include "boltor/boltor.h"
#include "raylib.h"

void gameLoop(void)
{
    const int minSize  = 20;
    const int maxSize  = 100;
    const int sizeStep = 1;

    static int fontSize = minSize;
    static bool goingUp = true;

    ClearBackground(RAYWHITE);
    DrawText("Boltor BEGINS!", 190, 200, fontSize, LIGHTGRAY);

    if (goingUp)
    {
        fontSize = fontSize + sizeStep;
    }
    else
    {
        fontSize = fontSize - sizeStep;
    }

    if (fontSize == minSize || fontSize == maxSize)
    {
        goingUp = !goingUp;
    }
}

int main(void)
{
    const int screenWidth  = 800;
    const int screenHeight = 450;
    const int targetFPS    = 60;

    return BoltorGame(screenWidth, screenHeight, targetFPS, "Hello from Raylib!", gameLoop);
}
