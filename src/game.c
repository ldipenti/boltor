#include "boltor/boltor.h"
#include "raylib.h"

typedef struct
{
    char *text;
    int size;
    Color color;
} VisualComponent;

typedef struct
{
    BTREntity entity;
    VisualComponent visual;
    BTR2DPositionComponent position;
    BTR2DMovementComponent speed;
} CharacterEntity;

typedef struct
{
    BTREntity entity;
    VisualComponent visual;
    BTR2DPositionComponent position;
} MovingBannerEntity;

typedef struct
{
    CharacterEntity hero;
    MovingBannerEntity banner;
} GameData;

void gameloop(void *data)
{
    const int minSize   = 20;
    const int maxSize   = 100;
    const int sizeStep  = 1;
    const int frameStep = 50;

    static bool goingUp   = true;
    static int frameCount = frameStep;

    GameData *gameData    = (GameData *) data;
    CharacterEntity *c    = &gameData->hero;
    MovingBannerEntity *b = &gameData->banner;

    if (goingUp)
    {
        b->visual.size = b->visual.size + sizeStep;
    }
    else
    {
        b->visual.size = b->visual.size - sizeStep;
    }

    if (b->visual.size == minSize || b->visual.size == maxSize)
    {
        goingUp = !goingUp;
    }

    frameCount = frameCount - 1;
    if (frameCount == 0)
    {
        frameCount  = frameStep;
        c->speed.dy = 15;
    }

    BTR2DPhysicsSystem(&c->position, &c->speed);

    // Rendering
    ClearBackground(RAYWHITE);
    DrawText(b->visual.text, b->position.x, b->position.y, b->visual.size, b->visual.color);
    DrawText(c->visual.text, c->position.x, 450 - c->visual.size - c->position.y, c->visual.size, c->visual.color);

    // Position reset
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
    MovingBannerEntity banner;

    hero.entity.id    = 1;
    hero.visual.text  = "@";
    hero.visual.size  = 20;
    hero.visual.color = RED;
    hero.position.x   = 0;
    hero.position.y   = 0;
    hero.speed.dx     = 5;
    hero.speed.dy     = 15;

    banner.entity.id    = 2;
    banner.visual.text  = "Boltor BEGINS!";
    banner.visual.size  = 20;
    banner.visual.color = LIGHTGRAY;
    banner.position.x   = 190;
    banner.position.y   = 200;

    GameData data        = {hero, banner};
    BTRGameData gamedata = {gameloop, (void *) &data};

    return BTRGame(screenWidth, screenHeight, targetFPS, "Hello from Raylib!", gamedata);
}
