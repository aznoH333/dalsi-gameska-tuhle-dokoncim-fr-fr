#include "player.h"
#include "entities.h"
#include "gameplay.h"
#include "gframework.h"
#include "level.h"
#include <stdlib.h>
#include "bullet.h"
#include "gameCamera.h"

Entity* initPlayer(int x, int y){
    Player* p = malloc(sizeof(Player));

    p->xVelocity = 0;
    p->yVelocity = 0;
    p->flip = 0;
    p->fireCooldown = 0;

    Entity* out = initEntity(x, y - 6, 16, 22, ENTITY_PLAYER, p, &playerUpdate, &playerOnCollide, &playerOnDestroy, &playerClean);

    return out;
}


const float GRAVITY = 0.1f;
const float JUMP_STRENGTH = 3.4f;
const int UPPER_BODY_OFFSET = -6;
const float WALK_ACCELERATION = 0.2f;
const float MAX_WALK_SPEED = 2.0f;
const int BULLET_SPAWN_OFFSET_Y = 10;
const int BULLET_SPAWN_OFFSET_X = 8;
const int FIRE_COOLDWON = 6;
void playerUpdate(Entity* this){
    Player* data = this->data;
    Gameplay* gameplay = getGameplay();
    EntityManager* entities = getEntityManager();

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

            data->flip = 0;
        }
        else if (IsKeyDown(KEY_A) && data->xVelocity > -MAX_WALK_SPEED){
            data->xVelocity -= WALK_ACCELERATION;
            data->flip = 1;
        }
        
        // wall collisions
        if (collidesWithLevel(gameplay->level,
            this->x + ((data->xVelocity > 0) * this->w) + (2 * boolToSign(data->xVelocity > 0.0f)) + data->xVelocity,
            this->y,
            1, 
            this->h - 1)){
            data->xVelocity = 0.0f;
        }
    }
    
    
    // vertical movement
    {
        
        if (!isTouchingGround){
            data->yVelocity += GRAVITY;
        }else {
            // fix y
            /*
            if (data->yVelocity != 0.0f){
                this->y += data->yVelocity;
                this->y = (int) this->y - ((int)this->y % 16);
                this->y += this->h;
            }
            */

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
    
    // shooting
    {
        if (IsKeyDown(KEY_SPACE) && data->fireCooldown == 0){
            addEntity(entities, initBullet(this->x + (boolToSign(data->flip) * -BULLET_SPAWN_OFFSET_X) , this->y + BULLET_SPAWN_OFFSET_Y, boolToSign(data->flip) * -4, ENTITY_PLAYER_PROJECTILE));
            data->fireCooldown = FIRE_COOLDWON;
        }
    }

    // update values
    {
        this->x += data->xVelocity;
        this->y += data->yVelocity;
        
        data->fireCooldown -= data->fireCooldown > 0;

        updateGameCameraPosition(getCameraManager(), this->x, this->y);
    }


    // draw
    {
        // choose animation sprites
        int upperSprite = 5;
        int lowerSprite = 0;

        // lower animations
        {
            // jumping
            if (!isTouchingGround){
                lowerSprite = 4;
            }
            
            // walking animation
            else if (fabs(data->xVelocity) > 1){
                lowerSprite = getAnimationSprite(1, 3, 5, getGlobalTimer());
            }
        }
        
        // upper animations
        {
            if (data->fireCooldown > (FIRE_COOLDWON >> 2)){
                upperSprite = 6;
            }
        }


        // draw sprites
        drawF(lowerSprite, this->x, this->y - UPPER_BODY_OFFSET, data->flip, LAYER_OBJECTS);
        drawF(upperSprite, this->x, this->y, data->flip, LAYER_OBJECTS);
    }
}

void playerOnCollide(Entity* this, Entity* other){

}

void playerOnDestroy(Entity* this){

}

void playerClean(Entity* this){

}