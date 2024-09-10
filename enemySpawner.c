#include "enemySpawner.h"
#include "enemy.h"
#include "spritedata.h"
#include "particleEffect.h"
#include "gameplay.h"
#include "gameCamera.h"

void initBasedOnType(EnemySpawner* data, int type){
    switch (type) {
        default: gLog(LOG_ERR, "Invalid spawner id [%d]", type); break;
        
        case SPAWNER_LIZARD_RED: 
            data->enemyId = ENEMY_LIZARD_RED;
            data->spawnSprite = SPRITE_START_ENTITIES;
            break;
        
        case SPAWNER_LIZARD_BLUE:
            data->enemyId = ENEMY_LIZARD_BLUE;
            data->spawnSprite = SPRITE_START_ENTITIES + 2;
            break;
        
        case SPAWNER_ROBOT_RED:
            data->enemyId = ENEMY_ROBOT_RED;
            data->spawnSprite = SPRITE_START_ENTITIES + 4;
            break;
        
        case SPAWNER_ROBOT_BLUE:
            data->enemyId = ENEMY_ROBOT_BLUE;
            data->spawnSprite = SPRITE_START_ENTITIES + 8;
            break;
    }
}

const int DEFAULT_COOLDOWN = 90;
const int SPAWN_ANIMATION_SPEED = 20;
void initBasedOnSpawnerType(EnemySpawner* data, int spawnerType){
    switch (spawnerType) {
        case SPAWNER_TYPE_SLOW:
            data->cooldown = DEFAULT_COOLDOWN;
            data->defaultCooldown = DEFAULT_COOLDOWN;
            data->enemySpawnCount = 15;
            break;
        case SPAWNER_TYPE_FAST:
            data->cooldown = 25;
            data->defaultCooldown = 25;
            data->enemySpawnCount = 5;
            break;
    }
}


Entity* initSpawner(int x, int y, int type, int spawnerType){
    EnemySpawner* data = malloc(sizeof(EnemySpawner));
    initBasedOnType(data, type);
    initBasedOnSpawnerType(data, spawnerType);

    data->direction = false;
    Entity* out = initEntity(x, y, 16, 16, ENTITY_OTHER, data, &spawnerUpdate, &spawnerOnCollide, &spawnerOnDestroy, &spawnerClean);
    return out;
}

void spawnerUpdate(Entity* this){
    EnemySpawner* data = this->data;

    data->cooldown--;

    if (data->cooldown == SPAWN_ANIMATION_SPEED){
        // init particle
        data->direction = getGameplay()->playerX > this->x;
        Entity* p = initStaticParticle(this->x, this->y, data->spawnSprite, SPAWN_ANIMATION_SPEED);
        makeParticleChangeTransparency(p, 0, 255);
        
        if (data->direction){
            flipParticle(p);
        }
        
        addEntity(getEntityManager(), p);
    }

    else if (data->cooldown == 0){
        data->cooldown = data->defaultCooldown;
        // spawn enemy
        Entity* enemy = initEnemyDirectional(this->x, this->y, data->enemyId, data->direction);
        addEntity(getEntityManager(), enemy);
        data->enemySpawnCount--;
        if (data->enemySpawnCount == 0){
            this->destroyFlag = DESTROY_NORMAL;
        }
    }

    if (!isOnScreen(getCameraManager(), this->x, this->y, this->w, this->h)){
        this->destroyFlag = DESTROY_NORMAL;
    }

}

void spawnerOnCollide(Entity* this, Entity* other){

}
void spawnerOnDestroy(Entity* this){

}
void spawnerClean(Entity* this){

}