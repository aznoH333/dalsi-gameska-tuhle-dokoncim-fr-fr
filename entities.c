
#include "entities.h"
#include "gameplay.h"
#include "gutil.h"
#include "gcollections.h"
#include <stdlib.h>
#include "enemy.h"
#include "enemySpawner.h"
#include "cannonTurret.h"
#include "levelExit.h"
#include "gameCamera.h"
#include "bossSpawner.h"

// Entity marker
EntityMarker* initEntityMarker(char* levelData, int markerIndex){
    EntityMarker* out = malloc(sizeof(EntityMarker));
    
    
    char x[2] = {levelData[markerIndex], levelData[markerIndex + 1]};
    out->x = parseStrToInt(x, 2);
    char y[2] = {levelData[markerIndex + 2], levelData[markerIndex + 3]};
    out->y = parseStrToInt(y, 2);
    char id[2] = {levelData[markerIndex + 4], levelData[markerIndex + 5]};
    out->id = parseStrToInt(id, 2);
    out->hasBeenActivated = false;

    return out;
}

EntityMarker* initEntityMarkerBasic(int id, int x, int y){
    EntityMarker* out = malloc(sizeof(EntityMarker));
    out->id = id;
    out->x = x;
    out->y = y;
    return out;
}


void saveEntityMarker(EntityMarker* marker, char* fileData, int markerIndex){
    writeIntAsChar(fileData, marker->x, 2, markerIndex);
    writeIntAsChar(fileData, marker->y, 2, markerIndex + 2);
    writeIntAsChar(fileData, marker->id, 2, markerIndex + 4);
}



int getEnemyTypeFromMarker(int markerId){
    switch (markerId) {
        default: return -1;
        // enemies
        case 9:     return ENEMY_LIZARD_RED;
        case 11:    return ENEMY_LIZARD_BLUE;

        case 13:    return ENEMY_ROBOT_RED;
        case 15:    return ENEMY_ROBOT_BLUE;

        case 28:    return ENEMY_SOLDIER_RED;
        case 29:    return ENEMY_SOLDIER_BLUE;

        case 30:    return ENEMY_LARGE_FLY_RED;
        case 31:    return ENEMY_LARGE_FLY_BLUE;

        case 26:    return ENEMY_SQUID_RED;
        case 27:    return ENEMY_SQUID_BLUE;
        //case 32:    return ENEMY_BOSS_RED;
        //case 33:    return ENEMY_BOSS_BLUE;
    }
}

int getEnemySpawnerType(int markerId){
    switch (markerId) {
        default: return -1;
        case 10: return SPAWNER_LIZARD_RED;
        case 12: return SPAWNER_LIZARD_BLUE;

        case 14: return SPAWNER_ROBOT_RED;
        case 16: return SPAWNER_ROBOT_BLUE;
    }
}




void activateEntityMarker(EntityMarker* marker){
    
    if (marker->hasBeenActivated){
        return;
    }

    marker->hasBeenActivated = true;
    
    // enemies
    int enemyType = getEnemyTypeFromMarker(marker->id);
    
    if (enemyType != -1){
        addEntity(getEntityManager(), initEnemy(marker->x * 16, marker->y * 16, enemyType));
        return;
    }   

    // enemy spawners
    int spawnerType = getEnemySpawnerType(marker->id);

    if (spawnerType != -1){
        

        addEntity(getEntityManager(), initSpawner(marker->x * 16, marker->y * 16, spawnerType, getCameraManager()->markerCameraType == CAMERA_POINT_AUTO));
        return;
    }

    // misc entities

    switch (marker->id) {
        case 1: // level exit
            addEntity(getEntityManager(), initExit(marker->x * 16, marker->y * 16));
            break;
        case 34: // water
            setWaterHeight(marker->y * 16);
            break;

        case 17:
        case 18:
        case 19:
        case 20: // cannon turret
            addEntity(getEntityManager(), initCannon(marker->x * 16, marker->y * 16, marker->id - 17));
            break;
        case 32: // boss spawner
        case 33:
            addEntity(getEntityManager(), initBossSpawner(marker->x * 16, marker->y * 16, marker->id - 32));
            break;
    }
    

    
}




void disposeEntityMarker(EntityMarker* marker){
    //free(marker);
}

// Entity
Entity* initEntity(int x, int y, int w, int h, int identifier, void* data, void (*updateFunction)(struct Entity*), void (*onCollide)(struct Entity*, struct Entity*), void (*onDestroy)(struct Entity*), void (*clean)(Entity*)){
    Entity* out = malloc(sizeof(Entity));
    
    out->x = x;
    out->y = y;
    out->w = w;
    out->h = h;
    out->extraIndex = -1;
    out->identifier = identifier;
    out->destroyFlag = DESTROY_NO;
    out->data = data;
    out->updateFunction = updateFunction;
    out->onCollide = onCollide;
    out->onDestroy = onDestroy;
    out->clean = clean;

    return out;
}




// Entity manager
EntityManager* entityManagerInstance = 0;
EntityManager* initEntityManager(){
    EntityManager* out = malloc(sizeof(EntityManager));

    out->entities = initVector();
    out->extraEntityData = initVector();

    return out;
}


EntityManager* getEntityManager(){
    if (entityManagerInstance == 0){
        entityManagerInstance = initEntityManager();
    }
    return entityManagerInstance;
}

void removeAllEntities(EntityManager* manager){
    for (int i = 0; i < manager->entities->elementCount; i++){
        Entity* ent = vectorGet(manager->entities, i);
        ent->clean(ent);
        free(ent->data);
    }
    vectorClear(manager->entities);
}

void unloadEntityManager(EntityManager* manager){
    removeAllEntities(manager);
    vectorFree(manager->entities);
    vectorFree(manager->extraEntityData);
    free(manager);
}

void updateEntityManager(EntityManager* manager){
    for (int i = 0; i < manager->entities->elementCount; i++){
        Entity* ent = vectorGet(manager->entities, i);
        // call update
        ent->updateFunction(ent);



        if (ent->identifier != ENTITY_OTHER){
            // collision check
            for (int j = 0; j < manager->entities->elementCount; j++){
                if (i != j){
                    Entity* other = vectorGet(manager->entities, j);

                    if (other->identifier != ENTITY_OTHER && checkBoxCollisions(ent->x, ent->y, ent->w, ent->h, other->x, other->y, other->w, other->h)){
                        ent->onCollide(ent, other);
                    }
                }
            }
        }
        

        // destroy
        if (ent->destroyFlag > 0){
            if (ent->destroyFlag == DESTROY_NORMAL){
                ent->onDestroy(ent);
            }
            ent->clean(ent);
            free(ent->data);
            free(ent);
            vectorRemove(manager->entities, i);
            i--;
        }
    }
}

void addEntity(EntityManager* manager, Entity* entity){
    vectorPush(manager->entities, entity);
}


int allocateExtraEntityData(EntityManager* manager, void* data){
    vectorPush(manager->extraEntityData, data);
    return manager->extraEntityData->elementCount - 1;
}

void removeExtraEntityData(EntityManager* manager, int extraDataIndex){
    vectorRemove(manager->extraEntityData, extraDataIndex);


    // decrement all indicies
    for (int i = 0; i < manager->entities->elementCount; i++){
        Entity* e = vectorGet(manager->entities, i);

        if (e->extraIndex > extraDataIndex){
            e->extraIndex--;
        }
    }
}

void* getExtraEntityData(EntityManager* manager, int extraDataIndex){
    return vectorGet(manager->extraEntityData, extraDataIndex);
}
