#pragma once

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
    int id;
} BTREntity;

void BTR2DPhysicsSystem(BTR2DPositionComponent *p, BTR2DMovementComponent *m);