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

#define SPAWNER_LIZARD_RED 0
#define SPAWNER_LIZARD_BLUE 1
#define SPAWNER_ROBOT_RED 2
#define SPAWNER_ROBOT_BLUE 3


Entity* initSpawner(int x, int y, int type);
void spawnerUpdate(Entity* this);
void spawnerOnCollide(Entity* this, Entity* other);
void spawnerOnDestroy(Entity* this);
void spawnerClean(Entity* this);



#endif