#include "levelExit.h" 
#include "spritedata.h"
#include "gframework.h"
#include "gameplay.h"

Entity* initExit(int x, int y){
    
    
    LevelExit* data = malloc(sizeof(LevelExit));
    
    
    Entity* out = initEntity(x, y, 16, 16, ENTITY_OTHER_INTERACTABLE, data, &exitUpdate, &exitOnCollide, &exitOnDestroy, &exitClean);

}
void exitUpdate(Entity* this){
    // fall
    if (!collidesWithLevel(getGameplay()->level, this->x, this->y, this->w, this->h)){
        this->y += 1.5f;
    }


    // draw
    draw(SPRITE_START_ENTITIES + 47, this->x, this->y, LAYER_OBJECTS);
}

void exitOnCollide(Entity* this, Entity* other){
    
    if (other->identifier == ENTITY_PLAYER){
        // exit level
        touchedLevelExit(getGameplay());
        
        //gLog(LOG_ERR, "TODO not implemented");
    }
}


void exitOnDestroy(Entity* this){

}
void exitClean(Entity* this){

}
