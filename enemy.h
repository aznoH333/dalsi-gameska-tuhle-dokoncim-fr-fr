#ifndef ENEMY
#define ENEMY

#include "entities.h"

struct Enemy{
    float moveSpeed;
    bool flipDirection;
    int enemyType;
    int health;
    int hurtTimer;
    int animationTimer;
    int animationFrameDuration;
    int animationFrame;

    float yVelocity;
    float xVelocity;
    float gravity;
};
typedef struct Enemy Enemy;

#define ENEMY_GREY_LIZARD 0


Entity* initEnemy(int x, int y, int type);
void enemyUpdate(Entity* this);
void enemyOnCollide(Entity* this, Entity* other);
void enemyOnDestroy(Entity* this);
void enemyClean(Entity* this);

#endif