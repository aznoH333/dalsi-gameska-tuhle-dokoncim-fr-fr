#include "player.h"
#include "entities.h"
#include "gameplay.h"
#include "gframework.h"
#include "level.h"
#include <stdlib.h>

Entity* initPlayer(int x, int y){
    Player* p = malloc(sizeof(Player));

    p->xVelocity = 0;
    p->yVelocity = 0;

    Entity* out = initEntity(x, y, 16, 20, ENTITY_PLAYER, p, &playerUpdate, &playerOnCollide, &playerOnDestroy, &playerClean);

    

    return out;
}


const float GRAVITY = 0.1f;
const float JUMP_STRENGTH = 2.5f;
const int UPPER_BODY_OFFSET = -6;
const float WALK_ACCELERATION = 0.2f;
const float MAX_WALK_SPEED = 2.0f;
void playerUpdate(Entity* this){
    Player* data = this->data;
    Gameplay* gameplay = getGameplay();

    bool isTouchingGround = collidesWithLevel(gameplay->level, this->x, this->y + this->h + data->yVelocity, this->w, 1);
    
    
    
    // horizontal movement
    {
        if (IsKeyDown(KEY_D) == IsKeyDown(KEY_A)){
            // decelerate
            data->xVelocity -= sign(data->xVelocity) * WALK_ACCELERATION;
            if (fabs(data->xVelocity) < 0.2f){
                data->xVelocity = 0.0f;
            }
        }
        else if (IsKeyDown(KEY_D) && data->xVelocity < MAX_WALK_SPEED){
            data->xVelocity += WALK_ACCELERATION;
        }
        else if (IsKeyDown(KEY_A) && data->xVelocity > -MAX_WALK_SPEED){
            data->xVelocity -= WALK_ACCELERATION;
        }
        // wall collisions
        if (collidesWithLevel(gameplay->level, this->x + data->xVelocity, this->y, this->w, this->h - 4)){
            data->xVelocity = 0.0f;
        }
    }
    
    
    // vertical movement
    {
        
        if (!isTouchingGround){
            data->yVelocity += GRAVITY;
        }else {
            // fix y
            
            if (data->yVelocity != 0.0f){
                this->y += data->yVelocity;
                this->y -= this->y % 16;
                this->y += 16;
            }
            

            data->yVelocity = 0.0f;

        }

        // ceiling collisions
        if (collidesWithLevel(gameplay->level, this->x, this->y - 1 + data->yVelocity, this->w, 1)){
            data->yVelocity = 0.0f;
        }

        if (IsKeyDown(KEY_W) && isTouchingGround){
            data->yVelocity -= JUMP_STRENGTH;
        }
    }
    
    

    // update values
    {
        this->x += data->xVelocity;
        this->y += data->yVelocity;
    }


    // draw
    {
        draw(0, this->x, this->y, LAYER_OBJECTS);
        draw(5, this->x, this->y + UPPER_BODY_OFFSET, LAYER_OBJECTS);
        setCameraPos((Vector2){this->x - 250, this->y - 150}); // temp camera movement
    }
}

void playerOnCollide(Entity* this, Entity* other){

}

void playerOnDestroy(Entity* this){

}

void playerClean(Entity* this){

}