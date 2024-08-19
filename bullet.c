#include "bullet.h"
#include "gframework.h"
#include "gameplay.h"
#include "spritedata.h"
#include "particleEffect.h"

Entity* initBullet(int x, int y, float xVelocity, float yVelocity, int sprite, int team, char flags){

    Bullet* b = malloc(sizeof(Bullet));

    b->xVelocity = xVelocity;
    b->yVelocity = yVelocity;
    b->sprite = sprite;
    b->lifeTime = 160;
    b->damage = 5;
    b->flags = flags;

    Entity* out = initEntity(x, y, 16, 8, team, b, &bulletUpdate, &bulletOnCollide, &bulletOnDestroy, &bulletClean);

    return out;
}

bool doesFlagApply(char flag, Bullet* data){
    return (data->flags & flag) > 0;
}

void bulletUpdate(Entity* this){
    Bullet* data = this->data;
    Gameplay* gameplay = getGameplay();

    // collisions
    {
        if (!doesFlagApply(BULLET_FLAG_PHASING, data) && collidesWithLevel(gameplay->level, this->x, this->y, this->w, this->h)){
            this->shouldDestroy = true;
        }
    }


    // update
    {
        this->x += data->xVelocity;
        this->y += data->yVelocity;
        data->lifeTime--;
        if (data->lifeTime <= 0){
            this->shouldDestroy = true;
        }
    }

    // draw
    {
        int sprite = data->sprite;

        if (doesFlagApply(BULLET_FLAG_ANIMATED, data)){
            sprite += getGlobalTimer() % 20 > 10;
        }
        
        drawF(sprite, this->x, this->y, data->xVelocity < 0, LAYER_OBJECTS);
    }
}

void bulletOnCollide(Entity* this, Entity* other){

}
void bulletOnDestroy(Entity* this){
    if (doesFlagApply(BULLET_FLAG_SPAWN_DECAL, this->data)){
        addEntity(getEntityManager(), initStaticParticle(this->x + GetRandomValue(-2, 2), this->y - 6 + GetRandomValue(-2, 2), SPRITE_START_EFFECTS + 22, 10));
    }
}

void bulletClean(Entity* this){

}
