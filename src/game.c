#include "boltor/boltor.h"
#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_ENTITY_CAPACITY 100

enum EntityTypes
{
    ENTITY_TEXT,
    ENTITY_CHARACTER
};

typedef struct _CharacterEntity
{
    BTRTextComponent visual;
    BTR2DPositionComponent position;
    BTR2DMovementComponent speed;
} CharacterEntity;

typedef struct _TextEntity
{
    BTRTextComponent visual;
    BTR2DPositionComponent position;
} TextEntity;

typedef struct _Entity
{
    int type;
    int id;
    void *data;
} Entity;

typedef struct _Registry
{
    BTR2DPositionComponent *positions[INITIAL_ENTITY_CAPACITY];
    BTR2DMovementComponent *speeds[INITIAL_ENTITY_CAPACITY];
    BTRTextComponent *texts[INITIAL_ENTITY_CAPACITY];
    Entity *entities[INITIAL_ENTITY_CAPACITY];
    int nextID;
    int capacity;
} Registry;

typedef struct _GameData
{
    Registry *registry;
    CharacterEntity *hero;
    TextEntity *banner, *fpscounter;
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
    Registry *reg      = gameData->registry;
    CharacterEntity *c = gameData->hero;
    TextEntity *b      = gameData->banner;
    TextEntity *fps    = gameData->fpscounter;

    const int frameStep   = 50;
    static int frameCount = frameStep;

    frameCount = frameCount - 1;
    if (frameCount == 0)
    {
        frameCount  = frameStep;
        c->speed.dy = 15;
    }

    // Player input handling
    if (IsKeyDown(KEY_RIGHT))
    {
        c->speed.dx += 1;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        c->speed.dx -= 1;
    };

    // Systems mutating component data
    for (int i = 0; i < reg->nextID; i++)
    {
        BTR2DPositionComponent *pos = reg->positions[i];
        BTR2DMovementComponent *mov = reg->speeds[i];
        if (pos != NULL && mov != NULL)
        {
            BTR2DPhysicsSystem(pos, mov);
        }
    }

    b->visual.refreshText(b);
    fps->visual.refreshText(fps);

    // Rendering
    ClearBackground(RAYWHITE);
    DrawText(b->visual.text, b->position.x, b->position.y, b->visual.size, b->visual.color);
    DrawText(c->visual.text, c->position.x, 450 - c->visual.size - c->position.y, c->visual.size, c->visual.color);
    DrawText(fps->visual.text, fps->position.x, fps->position.y, fps->visual.size, fps->visual.color);

    // Position reset
    if (c->speed.dx > 0 && c->position.x > 800)
    {
        c->position.x = 0;
    }
    else if (c->speed.dx < 0 && c->position.x < 0)
    {
        c->position.x = 800;
    }
}

Registry *newRegistry(void)
{
    Registry *reg = (Registry *) malloc(sizeof(Registry));
    for (int i = 0; i < INITIAL_ENTITY_CAPACITY; i++)
    {
        reg->positions[i] = NULL;
        reg->speeds[i]    = NULL;
        reg->texts[i]     = NULL;
        reg->entities[i]  = NULL;
    }
    reg->capacity = INITIAL_ENTITY_CAPACITY;
    reg->nextID   = 0;

    return reg;
}

void registryAdd(Registry *reg, Entity *e)
{
    if (reg->nextID < reg->capacity)
    {
        TraceLog(LOG_INFO, "Adding entity type %d - id: %d", e->type, reg->nextID);
        e->id                = reg->nextID;
        reg->entities[e->id] = e;
        reg->nextID += 1;

        switch (e->type)
        {
            case ENTITY_CHARACTER: {
                CharacterEntity *ce   = (CharacterEntity *) e->data;
                reg->positions[e->id] = &ce->position;
                reg->speeds[e->id]    = &ce->speed;
                reg->texts[e->id]     = &ce->visual;
                break;
            }
            case ENTITY_TEXT: {
                TextEntity *te        = (TextEntity *) e->data;
                reg->positions[e->id] = &te->position;
                reg->texts[e->id]     = &te->visual;
                break;
            }
            default:
                break;
        }
    }
    else
    {
        // FIXME: need to expand capacity
    }
}

CharacterEntity *newCharacterEntity(Registry *reg, int posX, int posY, char *c, int size, Color color)
{
    CharacterEntity *ce    = (CharacterEntity *) malloc(sizeof(CharacterEntity));
    ce->position.x         = posX;
    ce->position.y         = posY;
    ce->speed.dx           = 0;
    ce->speed.dy           = 0;
    ce->visual.text        = c;
    ce->visual.size        = size;
    ce->visual.color       = color;
    ce->visual.refreshText = NULL;

    Entity *e = (Entity *) malloc(sizeof(Entity));
    e->type   = ENTITY_CHARACTER;
    e->data   = ce;
    e->id     = 0;
    registryAdd(reg, e);

    return ce;
}

TextEntity *newTextEntity(Registry *reg, int posX, int posY, char *text, int size, Color color)
{
    TextEntity *te         = (TextEntity *) malloc(sizeof(TextEntity));
    te->position.x         = posX;
    te->position.y         = posY;
    te->visual.text        = text;
    te->visual.size        = size;
    te->visual.color       = color;
    te->visual.refreshText = NULL;

    Entity *e = (Entity *) malloc(sizeof(Entity));
    e->type   = ENTITY_TEXT;
    e->data   = te;
    e->id     = 0;
    registryAdd(reg, e);

    return te;
}

int main(void)
{
    const int screenWidth  = 800;
    const int screenHeight = 450;
    const int targetFPS    = 60;
    CharacterEntity *hero;
    TextEntity *banner;
    TextEntity *fpscounter;

    Registry *reg = newRegistry();

    hero           = newCharacterEntity(reg, 0, 0, "@", 20, RED);
    hero->speed.dx = 5;
    hero->speed.dy = 15;

    banner                     = newTextEntity(reg, 190, 200, "Boltor BEGINS!", 20, LIGHTGRAY);
    banner->visual.refreshText = &refreshBanner;

    fpscounter                     = newTextEntity(reg, 5, 5, NULL, 25, DARKGREEN);
    fpscounter->visual.refreshText = &refreshFPSCounter;

    GameData data        = {reg, hero, banner, fpscounter};
    BTRGameData gamedata = {gameloop, &data};

    return BTRGame(screenWidth, screenHeight, targetFPS, "Hello from Raylib!", gamedata);
}
