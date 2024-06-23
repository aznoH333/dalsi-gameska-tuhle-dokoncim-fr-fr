#include "player.h"
#include "entities.h"
#include "gframework.h"
#include <stdlib.h>

Entity* initPlayer(int x, int y){
    Player* p = malloc(sizeof(Player));
    
    
    
    
    Entity* out = initEntity(x, y, 16, 20, ENTITY_PLAYER, p, &playerUpdate, &playerOnCollide, &playerOnDestroy, &playerClean);
}


const int UPPER_BODY_OFFSET = -6;
void playerUpdate(Entity* this){
    Player* data = this->data;


    if (IsKeyDown(KEY_D)){
        this->x += 1;
    }
    

    draw(0, this->x, this->y, LAYER_OBJECTS);
    draw(5, this->x, this->y + UPPER_BODY_OFFSET, LAYER_OBJECTS);
    setCameraPos((Vector2){this->x - 250, this->y - 150}); // temp camera movement

}

void playerOnCollide(Entity* this, Entity* other){

}

void playerOnDestroy(Entity* this){

}

void playerClean(Entity* this){

}