#ifndef GAMEPLAY
#define GAMEPLAY

#include "level.h"
#include "entities.h"
#include "gframework.h"

struct Gameplay{
    Level* level;
    bool hasLoadedLevel;
};

typedef struct Gameplay Gameplay;


Gameplay* getGameplay();
void unloadGameplay(Gameplay* g);
void startLevel(Gameplay* g, const char* levelPath);
void updateGameplay(Gameplay* g);

#endif