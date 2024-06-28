#ifndef BULLET
#define BULLET

#include "entities.h"

struct Bullet{
    float velocity;
    int lifeTime;
};
typedef struct Bullet Bullet;

Entity* initBullet(int x, int y, float velocity, int team);
void bulletUpdate(Entity* this);
void bulletOnCollide(Entity* this, Entity* other);
void bulletOnDestroy(Entity* this);
void bulletClean(Entity* this);

#endif