#ifndef ENEMY
#define ENEMY

#include "entities.h"

struct Enemy{
    float moveSpeed;
    bool flipDirection;
    int enemyType;
    int health;
    int hurtTimer;
};
typedef struct Enemy Enemy;

#define ENEMY_GREY_LIZARD 0


Entity* initEnemy(int x, int y, int type);
void enemyUpdate(Entity* this);
void enemyOnCollide(Entity* this, Entity* other);
void enemyOnDestroy(Entity* this);
void enemyClean(Entity* this);

#endif