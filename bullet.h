#ifndef BULLET
#define BULLET

#include "entities.h"

struct Bullet{
    float xVelocity;
    float yVelocity;
    int sprite;
    int lifeTime;
    int damage;
};
typedef struct Bullet Bullet;

Entity* initBullet(int x, int y, float xVelocity, float yVelocity, int sprite, int team);
void bulletUpdate(Entity* this);
void bulletOnCollide(Entity* this, Entity* other);
void bulletOnDestroy(Entity* this);
void bulletClean(Entity* this);

#endif