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
    int targetWaterHeight;
    int startWaterHeight;
    float waterProgress;
    int currentPassiveMarkerEffect;
};

typedef struct Gameplay Gameplay;

#define MARKER_EFFECT_NONE 0
#define MARKER_EFFECT_FLY_GREY 1
#define MARKER_EFFECT_FLY_RED 2
#define MARKER_EFFECT_FLY_BLUE 3
#define MARKER_EFFECT_STOP 4
#define MARKER_EFFECT_CUSTOM_SCRIPT 5

Gameplay* getGameplay();
void unloadGameplay(Gameplay* g);
void disposeGameplay(Gameplay* g);
void startLevel(Gameplay* g, const char* levelPath);
void updateGameplay(Gameplay* g);
void setMarkerEffect(int markerEffect);
EntityMarker* getCollidingMarker(Gameplay* g, Entity* entity);
void setPlayerCoordinates(Gameplay* gameplay, float x, float y);
void setWaterHeight(int height);
#endif