#ifndef G_VECTOR
#define G_VECTOR

struct Vector{
    void** elements; 
    int elementCount;
    int allocatedSize;
};
typedef struct Vector Vector;


Vector initVector();
void vectorResize(Vector* v);
void vectorPush(Vector* v, void* element);
void vectorRemove(Vector* v, int index);
void* vectorGet(Vector* v, int index);
void vectorClear(Vector* v);
void vectorFree(Vector* v);

#endif