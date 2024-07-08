#ifndef GAMEPLAY
#define GAMEPLAY

#include "level.h"
#include "entities.h"
#include "gframework.h"

struct Gameplay{
    Level* level;
    bool hasLoadedLevel;
    float playerX;
    float playerY;
};

typedef struct Gameplay Gameplay;


Gameplay* getGameplay();
void unloadGameplay(Gameplay* g);
void disposeGameplay(Gameplay* g);
void startLevel(Gameplay* g, const char* levelPath);
void updateGameplay(Gameplay* g);
EntityMarker* getCollidingMarker(Gameplay* g, Entity* entity);
void setPlayerCoordinates(Gameplay* gameplay, float x, float y);

#endif