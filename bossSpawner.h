#ifndef BOSS_SPAWNER
#define BOSS_SPAWNER 

#include "entities.h"

struct BossSpawner{
    char bossType;
};
typedef struct BossSpawner BossSpawner;

Entity* initBossSpawner(int x, int y, char type);
void bossSpawnerUpdate(Entity* this);
void bossSpawnerOnCollide(Entity* this, Entity* other);
void bossSpawnerOnDestroy(Entity* this);
void bossSpawnerClean(Entity* this);

#endif