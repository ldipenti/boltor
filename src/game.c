#include "boltor/boltor.h"
#include "math.h"
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct
{
    BTREntity entity;
    TextComponent visual;
    BTR2DPositionComponent position;
    BTR2DMovementComponent speed;
} CharacterEntity;

typedef struct
{
    BTREntity entity;
    TextComponent visual;
    BTR2DPositionComponent position;
} TextEntity;

typedef struct
{
    CharacterEntity hero;
    TextEntity banner, fpscounter;
} GameData;

void refreshBanner(void *data)
{
    const int minSize   = 20;
    const int maxSize   = 100;
    const int sizeStep  = 1;
    const int frameStep = 50;

    static bool goingUp = true;

    TextEntity *b = (TextEntity *) data;

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
}

void refreshFPSCounter(void *data)
{
    TextEntity *counter = (TextEntity *) data;
    float dt            = GetFrameTime();
    if (dt > 0)
    {
        if (counter->visual.text != NULL)
        {
            free(counter->visual.text);
        }
        char *fpscount = (char *) malloc(20 * sizeof(char));
        sprintf(fpscount, "%d FPS", (int) floor(1.0 / dt));
        counter->visual.text = fpscount;
    }
}

void gameloop(void *data)
{
    GameData *gameData = (GameData *) data;
    CharacterEntity *c = &gameData->hero;
    TextEntity *b      = &gameData->banner;
    TextEntity *fps    = &gameData->fpscounter;

    const int frameStep   = 50;
    static int frameCount = frameStep;

    frameCount = frameCount - 1;
    if (frameCount == 0)
    {
        frameCount  = frameStep;
        c->speed.dy = 15;
    }

    BTR2DPhysicsSystem(&c->position, &c->speed);
    b->visual.refreshText(b);
    fps->visual.refreshText(fps);

    // Rendering
    ClearBackground(RAYWHITE);
    DrawText(b->visual.text, b->position.x, b->position.y, b->visual.size, b->visual.color);
    DrawText(c->visual.text, c->position.x, 450 - c->visual.size - c->position.y, c->visual.size, c->visual.color);
    DrawText(fps->visual.text, fps->position.x, fps->position.y, fps->visual.size, fps->visual.color);

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
    TextEntity banner;
    TextEntity fpscounter;

    hero.entity.id          = 1;
    hero.visual.text        = "@";
    hero.visual.size        = 20;
    hero.visual.color       = RED;
    hero.position.x         = 0;
    hero.position.y         = 0;
    hero.speed.dx           = 5;
    hero.speed.dy           = 15;
    hero.visual.refreshText = NULL;

    banner.entity.id          = 2;
    banner.visual.text        = "Boltor BEGINS!";
    banner.visual.size        = 20;
    banner.visual.color       = LIGHTGRAY;
    banner.position.x         = 190;
    banner.position.y         = 200;
    banner.visual.refreshText = &refreshBanner;

    fpscounter.entity.id          = 3;
    fpscounter.visual.text        = NULL;
    fpscounter.visual.color       = DARKGREEN;
    fpscounter.visual.size        = 25;
    fpscounter.position.x         = 5;
    fpscounter.position.y         = 5;
    fpscounter.visual.refreshText = &refreshFPSCounter;

    GameData data        = {hero, banner, fpscounter};
    BTRGameData gamedata = {gameloop, &data};

    return BTRGame(screenWidth, screenHeight, targetFPS, "Hello from Raylib!", gamedata);
}
