#ifndef ENTITIES
#define ENTITIES 

#include <stdlib.h>
#include "gutil.h"
#include <stdbool.h> 

struct EntityMarker{
    int x;
    int y;
    int id;
    bool hasBeenActivated;
};

typedef struct EntityMarker EntityMarker;

EntityMarker* initEntityMarker(char* levelData, int markerIndex);
EntityMarker* initEntityMarkerBasic(int id, int x, int y);
void saveEntityMarker(EntityMarker* marker, char* fileData, int markerIndex);
void disposeEntityMarker(EntityMarker* marker);

#endif