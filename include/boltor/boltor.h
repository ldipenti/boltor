#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int BoltorGame(int width, int height, int fps, char *title, void (*callback)(void));

#ifdef __cplusplus
}
#endif