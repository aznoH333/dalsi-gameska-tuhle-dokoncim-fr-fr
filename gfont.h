#ifndef GFONT
#define GFONT

#include "raylib.h"
#include "gframework.h"
#include "gutil.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>


#define CHARACTER_SIZE 8

void initFont();
void disposeFont();
void drawText(const char* text, int x, int y, float scale, Color color, int layer);
void drawTextF(const char* text, int x, int y, float scale, Color color, int layer, ...);


#endif