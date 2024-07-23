#include "enemySpawner.h"
#include "enemy.h"
#include "spritedata.h"
#include "particleEffect.h"

void initBasedOnType(EnemySpawner* data, int type){
    switch (type) {
        default: gLog(LOG_ERR, "Invalid spawner id [%d]", type); break;
        
        case SPAWNER_GREY_LIZARD: 
            data->enemyId = ENEMY_GREY_LIZARD;
            data->spawnSprite = SPRITE_START_ENTITIES;
            break;
        
        case SPAWNER_GREEN_LIZARD:
            data->enemyId = ENEMY_GREEN_LIZARD;
            data->spawnSprite = SPRITE_START_ENTITIES + 2;
            break;
        
        case SPAWNER_PINK_LIZARD:
            data->enemyId = ENEMY_PINK_LIZARD;
            data->spawnSprite = SPRITE_START_ENTITIES + 4;
            break;

        case SPAWNER_GREY_ROBOT:
            data->enemyId = ENEMY_GREY_ROBOT;
            data->spawnSprite = SPRITE_START_ENTITIES + 6;
            break;
        
        case SPAWNER_GREEN_ROBOT:
            data->enemyId = ENEMY_GREEN_ROBOT;
            data->spawnSprite = SPRITE_START_ENTITIES + 8;
            break;
    }
}

const int DEFAULT_COOLDOWN = 70;
const int SPAWN_ANIMATION_SPEED = 20;
Entity* initSpawner(int x, int y, int type){
    EnemySpawner* data = malloc(sizeof(EnemySpawner));
    initBasedOnType(data, type);
    data->cooldown = DEFAULT_COOLDOWN;
    data->enemySpawnCount = 25;
    Entity* out = initEntity(x, y, 16, 16, ENTITY_OTHER, data, &spawnerUpdate, &spawnerOnCollide, &spawnerOnDestroy, &spawnerClean);
    return out;
}

void spawnerUpdate(Entity* this){
    EnemySpawner* data = this->data;

    data->cooldown--;

    if (data->cooldown == SPAWN_ANIMATION_SPEED){
        // init particle
        Entity* p = initStaticParticle(this->x, this->y, data->spawnSprite, SPAWN_ANIMATION_SPEED);
        makeParticleChangeTransparency(p, 0, 255);
        addEntity(getEntityManager(), p);
    }

    else if (data->cooldown == 0){
        data->cooldown = DEFAULT_COOLDOWN;
        // spawn enemy
        Entity* enemy = initEnemy(this->x, this->y, data->enemyId);
        addEntity(getEntityManager(), enemy);
        data->enemySpawnCount--;
        if (data->enemySpawnCount == 0){
            this->shouldDestroy = true;
        }
    }

}

void spawnerOnCollide(Entity* this, Entity* other){

}
void spawnerOnDestroy(Entity* this){

}
void spawnerClean(Entity* this){

}