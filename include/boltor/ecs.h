#pragma once

#include <raylib.h>

typedef struct
{
    int x, y;
} BTR2DPositionComponent;

typedef struct
{
    int dx, dy;
} BTR2DMovementComponent;

typedef struct
{
    char *text;
    int size;
    Color color;
    void (*refreshText)(void *data);
} BTRTextComponent;

void BTR2DPhysicsSystem(BTR2DPositionComponent *p, BTR2DMovementComponent *m);