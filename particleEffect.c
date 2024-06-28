#include "particleEffect.h"
#include "gframework.h"

Entity* initStaticParticle(int x, int y, int sprite, int lifeTime){
    Particle* p = malloc(sizeof(Particle));


    p->isStatic = true;
    p->isAnimated = false;
    p->lifeTime = lifeTime;
    p->startFrame = sprite;
    p->currentSprite = 0;

    Entity* out = initEntity(x, y, 0, 0, ENTITY_OTHER, p, &particleUpdate, &particleOnCollide, &particleOnDestroy, &particleClean);

    return out;

}

void particleUpdate(Entity* this){
    Particle* data = this->data;

    // update movement
    {
        if (!data->isStatic){
            // todo
            this->x += data->xVelocity;
            this->y += data->yVelocity;
            this->y += data->gravity;

        }
    }

    // animation
    {
        if (data->isAnimated){
            // TODO
            data->timer--;

            if (data->timer <= 0){
                data->timer = data->frameDuration;
                data->currentSprite++;

                if (data->currentSprite + data->startFrame > data->endFrame){
                    data->currentSprite = 0;
                }
            }
        }
    }

    // timeout
    {
        data->lifeTime--;
        if (data->lifeTime == 0){
            this->shouldDestroy = true;
        }
    }
    

    // draw
    {
        draw(data->currentSprite + data->startFrame, this->x, this->y, LAYER_EFFECTS);
    }
}
void particleOnCollide(Entity* this, Entity* other){

}

void particleOnDestroy(Entity* this){

}

void particleClean(Entity* this){
    
}