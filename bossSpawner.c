#include "bossSpawner.h"
#include "gameplay.h"

Entity* initBossSpawner(int x, int y, char type){
    BossSpawner* data = malloc(sizeof(BossSpawner));
    data->bossType = type;

    Entity* this = initEntity(x, y, 0, 0, ENTITY_OTHER, data, &bossSpawnerUpdate, &bossSpawnerOnCollide, &bossSpawnerOnDestroy, &bossSpawnerClean);

    registerBossSpawner(getGameplay(), this);

    return this;
}

void bossSpawnerUpdate(Entity* this){}
void bossSpawnerOnCollide(Entity* this, Entity* other){}
void bossSpawnerOnDestroy(Entity* this){}
void bossSpawnerClean(Entity* this){}