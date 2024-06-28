#include "bullet.h"
#include "gframework.h"
#include "gameplay.h"
#include "spritedata.h"
#include "particleEffect.h"

Entity* initBullet(int x, int y, float velocity, int team){
    Bullet* b = malloc(sizeof(Bullet));

    b->velocity = velocity;

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
        this->x += data->velocity;
    }

    // draw
    {
        drawF(SPRITE_START_EFFECTS + 23, this->x, this->y, data->velocity < 0, LAYER_OBJECTS);
    }
}

void bulletOnCollide(Entity* this, Entity* other){

}
void bulletOnDestroy(Entity* this){
    addEntity(getEntityManager(), initStaticParticle(this->x + GetRandomValue(-2, 2), this->y - 6 + GetRandomValue(-2, 2), SPRITE_START_EFFECTS + 22, 10));
}
void bulletClean(Entity* this){

}
