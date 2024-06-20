#include "gvector.h"
#include <stdlib.h>
#include "gutil.h"

#define START_ELEMENT_COUNT 10

// tohle je chyba, ktere budu pozdeji litovat
// anyway...


Vector initVector(){
    Vector out;
    out.elements = malloc(sizeof(void*) * 10);
    out.elementCount = 0; 
    out.allocatedSize = 10;   
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

    free(v->elements[index]);

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
    free(v);
}
