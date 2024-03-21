#pragma once

#include "ecs.h"
#include <stdint.h>

int BoltorGame(int width, int height, int fps, char *title, void (*callback)(void *), void *c);