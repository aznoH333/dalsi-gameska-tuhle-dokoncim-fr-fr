#ifndef PLAYER
#define PLAYER

#include "entities.h"

struct Player{
    float xVelocity;
    float yVelocity;
    bool flip;
    int fireCooldown;
    int jumpHeightBuffer;
    unsigned char invincibilityTimer;
    unsigned char jumpInputBuffer;
    unsigned char kayoteTime;
};
typedef struct Player Player;




Entity* initPlayer(int x, int y, unsigned char invincibilityTimer);
void playerUpdate(Entity* this);
void playerOnCollide(Entity* this, Entity* other);
void playerOnDestroy(Entity* this);
void playerClean(Entity* this);

#endif