#pragma once

#include "ecs.h"

typedef struct
{
    void (*gameLoop)(void *data);
    void *data;
} BTRGameData;

int BTRGame(int width, int height, int fps, char *title, BTRGameData g);