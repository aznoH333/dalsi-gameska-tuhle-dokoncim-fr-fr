#ifndef GAMEPLAY
#define GAMEPLAY

#include "level.h"
#include "entities.h"
#include "gframework.h"

struct Gameplay{
    Level* level;
};
typedef struct Gameplay Gameplay;

Gameplay* startGameplay(const char* levelPath);
void stopGameplay(Gameplay* g);
void updateGameplay(Gameplay* g);

#endif