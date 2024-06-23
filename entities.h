#ifndef ENTITIES
#define ENTITIES 

#include <stdlib.h>
#include "gutil.h"
#include <stdbool.h> 
#include "gvector.h"

struct EntityMarker{
    int x;
    int y;
    int id;
    bool hasBeenActivated;
};

typedef struct EntityMarker EntityMarker;



struct Entity{
    int x;
    int y;
    int w;
    int h;
    int identifier;
    bool shouldDestroy;
    void* data;
    void (*updateFunction)(struct Entity*, void*);
    void (*onCollide)(struct Entity*, void*, struct Entity*);
    void (*onDestroy)(struct Entity*, void*);
};
typedef struct Entity Entity;



// entity markers
EntityMarker* initEntityMarker(char* levelData, int markerIndex);
EntityMarker* initEntityMarkerBasic(int id, int x, int y);
void saveEntityMarker(EntityMarker* marker, char* fileData, int markerIndex);
void disposeEntityMarker(EntityMarker* marker);

// entities
Entity* initEntity(int x, int y, int w, int h, int identifier, void* data, void (*updateFunction)(struct Entity*, void*), void (*onCollide)(struct Entity*, void*, struct Entity*), void (*onDestroy)(struct Entity*, void*));


// entity manager
struct EntityManager{
    Vector* entities;
};
typedef struct EntityManager EntityManager;

EntityManager* getEntityManager();
void unloadEntityManager(EntityManager* manager);
void updateEntityManager(EntityManager* manager);



#endif