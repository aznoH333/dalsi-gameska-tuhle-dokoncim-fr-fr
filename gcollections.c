#include "gcollections.h"
#include <stdlib.h>
#include "gutil.h"




//------------------------------------------------------------------------------------
// Vector
//------------------------------------------------------------------------------------

#define START_ELEMENT_COUNT 10

Vector* initVector(){
    Vector* out = malloc(sizeof(Vector));
    out->elements = malloc(sizeof(void*) * 10);
    out->elementCount = 0; 
    out->allocatedSize = 10;   
    return out;
}

void vectorResize(Vector* v){
    int newSize = v->allocatedSize * 10;
    v->elements = realloc(v->elements, newSize * sizeof(void*));
    if (v->elements == NULL){
        gLog(LOG_ERR, "Vector couldn't realloc");
    }
    
    v->allocatedSize = newSize;
}

void vectorPush(Vector* v, void* element){
    
    if (v->elementCount + 1 > v->allocatedSize){
        vectorResize(v);
    }
    v->elements[v->elementCount++] = element;
}

void vectorRemove(Vector* v, int index){
    if (index < 0 || index > v->elementCount){
        gLog(LOG_ERR, "Vector out of bounds access {%d}", index);
    }

    

    for (int i = index; i < v->elementCount - 1; i++){
        v->elements[i] = v->elements[i+1];
    }
    v->elementCount--;
}

void* vectorGet(Vector* v, int index){
    if (index < 0 || index > v->elementCount){
        gLog(LOG_ERR, "Vector out of bounds access {%d}", index);
    }

    return v->elements[index];
}

void vectorClear(Vector* v){
    
    for (int i = 0; i < v->elementCount; i++){
        free(v->elements[i]);
    }
    
    v->elementCount = 0;
}

void vectorFree(Vector* v){
    vectorClear(v);
    free(v->elements);
    free(v);
}

//------------------------------------------------------------------------------------
// Map
//------------------------------------------------------------------------------------


Map* initMap(){
    Map* out = malloc(sizeof(Map));

    out->elements = initVector();
    out->indexingElements = initVector();

    return out;
}


int mapIndexOfKey(Map* map, const char* key){
    for (int i = 0; i < map->indexingElements->elementCount; i++){
        char* str = vectorGet(map->indexingElements, i);

        if (strEquals(str, key)){
            return i;
        }
    }
    return -1;
}


void mapPut(Map* map, char* key, void* value){
    
    // check if key exists
    if (mapIndexOfKey(map, key) != -1){
        gLog(LOG_ERR, "Map key [%s] already exists", key);
    }
    

    vectorPush(map->indexingElements, key);
    vectorPush(map->elements, value);
}

void* mapGet(Map* map, const char* key){
    int index = mapIndexOfKey(map, key);
    if (index == -1){
        gLog(LOG_ERR, "Key [%s] not found", key);
    }

    return vectorGet(map->elements, index);
}

void mapFree(Map* map){
    vectorFree(map->elements);
    vectorFree(map->indexingElements);
    free(map);
}

void mapFreeConstKeys(Map* map){
    vectorFree(map->elements);
    free(map->indexingElements->elements);
    free(map->indexingElements);
    free(map);
}

void mapFreeConstKeysConstValues(Map* map){
    free(map->elements->elements);
    free(map->elements);
    free(map->indexingElements->elements);
    free(map->indexingElements);
    free(map);
}