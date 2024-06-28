
#include "entities.h"
#include "gutil.h"
#include "gvector.h"
#include <stdlib.h>


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

    return out;
}


EntityManager* getEntityManager(){
    if (entityManagerInstance == 0){
        entityManagerInstance = initEntityManager();
    }
    return entityManagerInstance;
}

void removeAllEntities(EntityManager* manager){
    for (int i = 0; i < manager->entities.elementCount; i++){
        Entity* ent = vectorGet(&manager->entities, i);
        ent->clean(ent);
        free(ent->data);
    }
    vectorClear(&manager->entities);
    vectorFree(&manager->entities);
}

void unloadEntityManager(EntityManager* manager){
    removeAllEntities(manager);
    
    free(manager);
}

void updateEntityManager(EntityManager* manager){
    for (int i = 0; i < manager->entities.elementCount; i++){
        Entity* ent = vectorGet(&manager->entities, i);
        // call update
        ent->updateFunction(ent);



        if (ent->identifier != ENTITY_OTHER){
            // collision check
            for (int j = 0; j < manager->entities.elementCount; j++){
                if (i != j){
                    Entity* other = vectorGet(&manager->entities, j);

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
            vectorRemove(&manager->entities, i);
            i--;
        }
    }
}

void addEntity(EntityManager* manager, Entity* entity){
    vectorPush(&manager->entities, entity);
}
