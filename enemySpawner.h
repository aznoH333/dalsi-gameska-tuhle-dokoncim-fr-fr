#ifndef ENEMY_SPAWNER
#define ENEMY_SPAWNER


#include "entities.h"


struct EnemySpawner{
    int enemyId;
    int spawnSprite;
    int enemySpawnCount;
    int cooldown;
    char defaultCooldown;
    bool direction;
};

typedef struct EnemySpawner EnemySpawner;

#define SPAWNER_LIZARD_RED 0
#define SPAWNER_LIZARD_BLUE 1
#define SPAWNER_ROBOT_RED 2
#define SPAWNER_ROBOT_BLUE 3

#define SPAWNER_TYPE_FAST 0
#define SPAWNER_TYPE_SLOW 1

Entity* initSpawner(int x, int y, int type, int spawnerType);
void spawnerUpdate(Entity* this);
void spawnerOnCollide(Entity* this, Entity* other);
void spawnerOnDestroy(Entity* this);
void spawnerClean(Entity* this);



#endif