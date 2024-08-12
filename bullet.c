#include "bullet.h"
#include "gframework.h"
#include "gameplay.h"
#include "spritedata.h"
#include "particleEffect.h"

Entity* initBullet(int x, int y, float xVelocity, float yVelocity, int sprite, int team){

    Bullet* b = malloc(sizeof(Bullet));

    b->xVelocity = xVelocity;
    b->yVelocity = yVelocity;
    b->sprite = sprite;
    b->lifeTime = 160;
    b->damage = 5;

    Entity* out = initEntity(x, y, 16, 8, team, b, &bulletUpdate, &bulletOnCollide, &bulletOnDestroy, &bulletClean);

    return out;
}

void bulletUpdate(Entity* this){
    Bullet* data = this->data;
    Gameplay* gameplay = getGameplay();

    // collisions
    {
        if (collidesWithLevel(gameplay->level, this->x, this->y, this->w, this->h)){
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
        drawF(data->sprite, this->x, this->y, data->xVelocity < 0, LAYER_OBJECTS);
    }
}

void bulletOnCollide(Entity* this, Entity* other){

}
void bulletOnDestroy(Entity* this){
    addEntity(getEntityManager(), initStaticParticle(this->x + GetRandomValue(-2, 2), this->y - 6 + GetRandomValue(-2, 2), SPRITE_START_EFFECTS + 22, 10));
}
void bulletClean(Entity* this){

}
