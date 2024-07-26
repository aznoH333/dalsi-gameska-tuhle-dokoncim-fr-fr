#ifndef ENEMY_SPAWNER
#define ENEMY_SPAWNER


#include "entities.h"


struct EnemySpawner{
    int enemyId;
    int spawnSprite;
    int enemySpawnCount;
    int cooldown;
    bool direction;
};

typedef struct EnemySpawner EnemySpawner;

#define SPAWNER_GREY_LIZARD 0
#define SPAWNER_GREEN_LIZARD 1
#define SPAWNER_PINK_LIZARD 2
#define SPAWNER_GREY_ROBOT 3
#define SPAWNER_GREEN_ROBOT 4


Entity* initSpawner(int x, int y, int type);
void spawnerUpdate(Entity* this);
void spawnerOnCollide(Entity* this, Entity* other);
void spawnerOnDestroy(Entity* this);
void spawnerClean(Entity* this);



#endif