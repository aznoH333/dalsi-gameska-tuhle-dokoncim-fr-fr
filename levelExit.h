#ifndef LEVEL_EXIT
#define LEVEL_EXIT

#include "entities.h"

struct LevelExit{
    char animationTimer;
};
typedef struct LevelExit LevelExit;

Entity* initExit(int x, int y);
void exitUpdate(Entity* this);
void exitOnCollide(Entity* this, Entity* other);
void exitOnDestroy(Entity* this);
void exitClean(Entity* this);

#endif