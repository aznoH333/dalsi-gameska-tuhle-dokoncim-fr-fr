#include "bullet.h"
#include "gframework.h"
#include "gameplay.h"

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
        drawF(131, this->x, this->y, data->velocity < 0, LAYER_OBJECTS);
    }
}

void bulletOnCollide(Entity* this, Entity* other){

}
void bulletOnDestroy(Entity* this){

}
void bulletClean(Entity* this){

}
