#include "levelExit.h" 
#include "spritedata.h"
#include "gframework.h"
#include "gameplay.h"

Entity* initExit(int x, int y){
    
    
    LevelExit* data = malloc(sizeof(LevelExit));
    data->animationTimer = 0;
    
    Entity* out = initEntity(x, y, 16, 16, ENTITY_OTHER_INTERACTABLE, data, &exitUpdate, &exitOnCollide, &exitOnDestroy, &exitClean);

}




#define ANIMATION_TIMER_MAX 60
#define FLASH_TIMER 16
#define ANIMATION_FRAMES 7
void exitUpdate(Entity* this){
    LevelExit* data = this->data;
    
    // fall
    if (!collidesWithLevel(getGameplay()->level, this->x, this->y, this->w, this->h)){
        this->y += 1.5f;
    }

    // animation
    int sprite = 0;

    data->animationTimer += (data->animationTimer == 0) * ANIMATION_TIMER_MAX;
    data->animationTimer -= data->animationTimer > 0;

    sprite = floorf(((float)data->animationTimer / FLASH_TIMER) * ANIMATION_FRAMES * (data->animationTimer <= FLASH_TIMER));

    // draw
    draw(SPRITE_START_ENTITIES + 54 - sprite, this->x, this->y, LAYER_OBJECTS);
}

void exitOnCollide(Entity* this, Entity* other){
    
    if (other->identifier == ENTITY_PLAYER){
        // exit level
        touchedLevelExit(getGameplay());
        playSound("victory.wav");
        this->destroyFlag = DESTROY_NORMAL;
        stopMusic();
        //playSound("win.wav");
        
        //gLog(LOG_ERR, "TODO not implemented");
    }
}


void exitOnDestroy(Entity* this){

}
void exitClean(Entity* this){

}
