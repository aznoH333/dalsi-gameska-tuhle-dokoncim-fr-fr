#ifndef G_VECTOR
#define G_VECTOR


// vector
struct Vector{
    void** elements; 
    int elementCount;
    int allocatedSize;
};
typedef struct Vector Vector;


Vector* initVector();
void vectorResize(Vector* v);
void vectorPush(Vector* v, void* element);
void vectorRemove(Vector* v, int index);
void* vectorGet(Vector* v, int index);
void vectorClear(Vector* v);
void vectorFree(Vector* v);

// map
struct Map{
    Vector* elements;
    Vector* indexingElements;
};
typedef struct Map Map;

Map* initMap();
void mapPut(Map* map, char* key, void* value);
void* mapGet(Map* map, const char* key);
void mapFree(Map* map);
void mapFreeConstKeys(Map* map);
void mapFreeConstKeysConstValues(Map* map);

#endif