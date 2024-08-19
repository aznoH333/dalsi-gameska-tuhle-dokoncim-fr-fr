#ifndef BULLET
#define BULLET

#include "entities.h"

struct Bullet{
    float xVelocity;
    float yVelocity;
    int sprite;
    int lifeTime;
    int damage;
    char flags;
};
typedef struct Bullet Bullet;

#define BULLET_FLAG_SPAWN_DECAL 0b001
#define BULLET_FLAG_PHASING 0b0010
#define BULLET_FLAG_ANIMATED 0b100

Entity* initBullet(int x, int y, float xVelocity, float yVelocity, int sprite, int team, char flags);
void bulletUpdate(Entity* this);
void bulletOnCollide(Entity* this, Entity* other);
void bulletOnDestroy(Entity* this);
void bulletClean(Entity* this);

#endif