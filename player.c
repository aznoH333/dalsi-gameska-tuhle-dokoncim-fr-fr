#include "player.h"
#include "entities.h"
#include "gameplay.h"
#include "gframework.h"
#include "level.h"
#include <stdlib.h>
#include "bullet.h"
#include "gameCamera.h"
#include "spritedata.h"



#define JUMP_INPUT_BUFFER 10
#define KAYOTE_TIME 10


#define WALK_LEFT_KEY KEY_A
#define WALK_RIGHT_KEY KEY_D
#define JUMP_KEY KEY_W 
#define SHOOT_KEY KEY_SPACE

Entity* initPlayer(int x, int y, unsigned char invincibilityTimer){
    Player* p = malloc(sizeof(Player));

    p->xVelocity = 0;
    p->yVelocity = 0;
    p->flip = 0;
    p->fireCooldown = 0;
    p->jumpHeightBuffer = 0;
    p->invincibilityTimer = invincibilityTimer;
    p->jumpInputBuffer = 0;
    p->kayoteTime = 0;

    Entity* out = initEntity(x, y - 6, 16, 22, ENTITY_PLAYER, p, &playerUpdate, &playerOnCollide, &playerOnDestroy, &playerClean);

    return out;
}


bool shouldJump(Player* this){
    return this->jumpInputBuffer > 0;
}

const float GRAVITY = 0.2f;
const float JUMP_STRENGTH = 3.4f;
const int UPPER_BODY_OFFSET = -6;
const float WALK_ACCELERATION = 0.2f;
const float MAX_WALK_SPEED = 2.0f;
const int BULLET_SPAWN_OFFSET_Y = 10;
const int BULLET_SPAWN_OFFSET_X = 8;
const int FIRE_COOLDWON = 6;
const int JUMP_BUFFER_LENGTH = 10;
void playerUpdate(Entity* this){
    Player* data = this->data;
    Gameplay* gameplay = getGameplay();
    EntityManager* entities = getEntityManager();

    bool isTouchingGround = collidesWithLevel(gameplay->level, this->x, this->y + this->h + data->yVelocity, this->w, 1);
    
    
    // horizontal movement
    {
        if (IsKeyDown(WALK_RIGHT_KEY) == IsKeyDown(WALK_LEFT_KEY)){
            // decelerate
            data->xVelocity -= sign(data->xVelocity) * WALK_ACCELERATION;
            if (fabs(data->xVelocity) < 0.2f){
                data->xVelocity = 0.0f;
            }
        }
        else if (IsKeyDown(WALK_RIGHT_KEY) && data->xVelocity < MAX_WALK_SPEED){
            data->xVelocity += WALK_ACCELERATION;

            data->flip = 0;
        }
        else if (IsKeyDown(WALK_LEFT_KEY) && data->xVelocity > -MAX_WALK_SPEED){
            data->xVelocity -= WALK_ACCELERATION;
            data->flip = 1;
        }
        
        // wall collisions
        if (collidesWithLevel(gameplay->level,
            this->x + ((data->xVelocity > 0) * (this->w + 1)) + (2 * boolToSign(data->xVelocity > 0.0f)) + data->xVelocity,
            this->y,
            1, 
            this->h - 1)){
            data->xVelocity = 0.0f;
        }
    }
    
    
    // vertical movement
    {
        if (!isTouchingGround){
            if (data->jumpHeightBuffer == 0){
                data->yVelocity += GRAVITY;
            }
        }else {
            // fix y
            
            if (data->yVelocity > 1.0f){
                this->y = (int) this->y - ((int) this->y % 16) + (this->h - 16) + 3; // proč +3 já nevím, ale funguje to perfektně
            }
            /*
            if (data->yVelocity != 0.0f){
            }*/
            

            data->yVelocity = 0.0f;

        }

        // ceiling collisions
        if (collidesWithLevel(gameplay->level, this->x, this->y + data->yVelocity, this->w, 1)){
            data->yVelocity = 0.0f;
        }
        // jumping
        if (shouldJump(data) && data->kayoteTime > 0){
            data->yVelocity = -JUMP_STRENGTH;
            data->jumpHeightBuffer = JUMP_BUFFER_LENGTH;
            playSound("walk.wav");
            data->kayoteTime = 0;
            isTouchingGround = false;
        }

        // height buffering should still wait for actual inputs
        if (IsKeyDown(JUMP_KEY) && data->jumpHeightBuffer > 0){
            data->jumpHeightBuffer--;
        }else {
            data->jumpHeightBuffer = 0;
        }

        // update input buffer
        if (IsKeyDown(JUMP_KEY)){
            data->jumpInputBuffer = JUMP_INPUT_BUFFER;
        }
    }
    
    // shooting
    {
        if (IsKeyDown(SHOOT_KEY) && data->fireCooldown == 0){
            playSoundVolume("shoot.wav", 0.3f);
            addEntity(entities, initBullet(this->x + (boolToSign(data->flip) * -BULLET_SPAWN_OFFSET_X) , this->y + BULLET_SPAWN_OFFSET_Y, boolToSign(data->flip) * -4, 0, SPRITE_START_EFFECTS + 23, ENTITY_PLAYER_PROJECTILE, BULLET_FLAG_SPAWN_DECAL));
            data->fireCooldown = FIRE_COOLDWON;
        }
    }

    // update values
    {
        this->x += data->xVelocity;
        this->y += data->yVelocity;

        data->invincibilityTimer -= data->invincibilityTimer > 0;
        
        data->fireCooldown -= data->fireCooldown > 0;

        CameraManager* cameraMan = getCameraManager();

        updateGameCameraPosition(cameraMan, this->x, this->y);

        // update gameplay
        setPlayerCoordinates(gameplay, this->x, this->y, isTouchingGround);

        data->jumpInputBuffer -= data->jumpInputBuffer > 0;
        data->kayoteTime -= data->kayoteTime > 0;
        if (isTouchingGround) data->kayoteTime = KAYOTE_TIME;
        // check if should die
        if (!isOnScreen(cameraMan, this->x, this->y, this->w, this->h)){
            playerJustDied(getGameplay());
            
            this->destroyFlag = DESTROY_NORMAL;
        }
    }


    // draw
    if (data->invincibilityTimer % 6 < 3){
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
    if (((Player*)this->data)->invincibilityTimer == 0 && (other->identifier == ENTITY_ENEMY || other->identifier == ENTITY_ENEMY_PROJECTILE)){
        playerJustDied(getGameplay());
        
        this->destroyFlag = DESTROY_NORMAL;
    }
}

void playerOnDestroy(Entity* this){

}

void playerClean(Entity* this){

}