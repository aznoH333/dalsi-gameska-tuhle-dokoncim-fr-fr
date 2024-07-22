
#include "entities.h"
#include "gutil.h"
#include "gcollections.h"
#include <stdlib.h>
#include "enemy.h"


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

void activateEntityMarker(EntityMarker* marker){
    
    if (marker->hasBeenActivated){
        return;
    }

    marker->hasBeenActivated = true;
    // hardcoded mess
    // no real way to make it better tho
    // ... you could make a lookup table dumbass >:(
    int enemyType = -1;
    
    switch (marker->id) {
        default: break;

        case 9:     enemyType = ENEMY_GREY_LIZARD; break;
        case 11:    enemyType = ENEMY_GREEN_LIZARD; break;
        case 13:    enemyType = ENEMY_PINK_LIZARD; break;

        case 15:    enemyType = ENEMY_GREY_ROBOT; break;
        case 17:    enemyType = ENEMY_GREEN_ROBOT; break;

        case 31:    enemyType = ENEMY_GREEN_SOLDIER; break;
        case 32:    enemyType = ENEMY_GREY_SOLDIER; break;
        case 33:    enemyType = ENEMY_BLUE_SOLDIER; break;
        case 34:    enemyType = ENEMY_RED_SOLDIER; break;
        
    }
    if (enemyType != -1){
        addEntity(getEntityManager(), initEnemy(marker->x * 16, marker->y * 16, enemyType));
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
    out->shouldDestroy = false;
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
        if (ent->shouldDestroy){
            ent->onDestroy(ent);
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
