#include "boltor/boltor.h"
#include "raylib.h"

typedef struct
{
    BTREntity entity;
    BTR2DPositionComponent position;
    BTR2DMovementComponent speed;
} CharacterEntity;

void gameLoop(void *data)
{
    const int minSize   = 20;
    const int maxSize   = 100;
    const int sizeStep  = 1;
    const int frameStep = 50;

    static int fontSize   = minSize;
    static bool goingUp   = true;
    static int frameCount = frameStep;

    CharacterEntity *c = (CharacterEntity *) data;

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

    frameCount = frameCount - 1;
    if (frameCount == 0)
    {
        frameCount  = frameStep;
        c->speed.dy = 15;
    }
    TwoDPhysicsSystem(&c->position, &c->speed);
    DrawText("@", c->position.x, 450 - minSize - c->position.y, minSize, RED);
    if (c->position.x > 800)
    {
        c->position.x = 0;
    }
}

int main(void)
{
    const int screenWidth  = 800;
    const int screenHeight = 450;
    const int targetFPS    = 60;
    CharacterEntity hero;

    hero.entity.id  = 1;
    hero.position.x = 0;
    hero.position.y = 0;
    hero.speed.dx   = 5;
    hero.speed.dy   = 15;

    return BoltorGame(screenWidth, screenHeight, targetFPS, "Hello from Raylib!", gameLoop, (void *) &hero);
}
