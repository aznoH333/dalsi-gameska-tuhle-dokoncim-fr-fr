#ifndef EXTRA_GRAPHICS
#define EXTRA_GRAPHICS

#include "entities.h"

struct ExtraGraphics{
    int lifeTime;
    int type;
};
typedef struct ExtraGraphics ExtraGraphics;

#define GRAPHICS_DEATH_SMALL 0
#define GRAPHICS_DEATH_ROBOT 1
#define GRAPHICS_DEATH_LARGE 2
#define GRAPHICS_DEATH_LARGE_ROBOT 3

Entity* initExtraGraphic(int x, int y, int type);
void graphicOnCollide(Entity* data, Entity* other);
void graphicOnDestroy(Entity* data);
void graphicClean(Entity* data);

#endif