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
    float playerRespawnX;
    float playerRespawnY;
    int targetWaterHeight;
    int startWaterHeight;
    float waterProgress;
    int currentPassiveMarkerEffect;
    int respawnCount;
    char respawnTimer;
};

typedef struct Gameplay Gameplay;

#define MARKER_EFFECT_NONE 0
#define MARKER_EFFECT_FLY_RED 1
#define MARKER_EFFECT_FLY_BLUE 2
#define MARKER_EFFECT_STOP 3
#define MARKER_EFFECT_CUSTOM_SCRIPT 4
#define DEFAULT_PLAYER_HP 2

Gameplay* getGameplay();
void unloadGameplay(Gameplay* g);
void disposeGameplay(Gameplay* g);
void startLevel(Gameplay* g, const char* levelPath);
void updateGameplay(Gameplay* g);
void setMarkerEffect(int markerEffect);
EntityMarker* getCollidingMarker(Gameplay* g, Entity* entity);
void setPlayerCoordinates(Gameplay* gameplay, float x, float y, bool isGrounded);
void setWaterHeight(int height);
void touchedLevelExit(Gameplay* this);
void resetWater();


void playerJustDied(Gameplay* this);
#endif