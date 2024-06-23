#include "player.h"
#include "entities.h"
#include "gframework.h"
#include <stdlib.h>

Entity* initPlayer(int x, int y){
    Player* p = malloc(sizeof(Player));
    
    
    
    
    Entity* out = initEntity(x, y, 16, 20, ENTITY_PLAYER, p, &playerUpdate, &playerOnCollide, &playerOnDestroy);
}


const int UPPER_BODY_OFFSET = -6;
void playerUpdate(Entity* this){
    draw(0, this->x, this->y, LAYER_OBJECTS);
    draw(5, this->x, this->y + UPPER_BODY_OFFSET, LAYER_OBJECTS);
}

void playerOnCollide(Entity* this, Entity* other){

}

void playerOnDestroy(Entity* this){

}