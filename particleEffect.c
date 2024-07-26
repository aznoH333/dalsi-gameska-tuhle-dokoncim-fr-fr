#include "particleEffect.h"
#include "gframework.h"
#include <stdbool.h>

Entity* initStaticParticle(int x, int y, int sprite, int lifeTime){
    Particle* p = malloc(sizeof(Particle));


    p->isStatic = true;
    p->isAnimated = false;
    p->lifeTime = lifeTime;
    p->startFrame = sprite;
    p->currentSprite = 0;
    p->startingTransparency = 255;
    p->endTransparency = 255;
    p->startingLifeTime = lifeTime;
    p->isFlipped = false;

    Entity* out = initEntity(x, y, 0, 0, ENTITY_OTHER, p, &particleUpdate, &particleOnCollide, &particleOnDestroy, &particleClean);

    return out;

}

void makeParticleAnimated(Entity* particle, int endFrame, int frameDuration){
    Particle* p = particle->data;

    p->isAnimated = true;
    p->timer = frameDuration;
    p->endFrame = endFrame;
    p->frameDuration = frameDuration;
}

void makeParticleMove(Entity* particle, float xVelocity, float yVelocity, float gravity){
    Particle* p = particle->data;
    
    p->isStatic = false;
    p->xVelocity = xVelocity;
    p->yVelocity = yVelocity;
    p->gravity = gravity;
}

void makeParticleAnimatedSingleLoop(Entity* particle, int endFrame){
    Particle* p = particle->data;
    p->isAnimated = true;
    p->endFrame = endFrame;
    p->frameDuration = p->lifeTime / (p->endFrame - p->startFrame);
    p->timer = p->frameDuration;

}

void makeParticleChangeTransparency(Entity* particle, int startingTransparency, int endTransparency){
    Particle* p = particle->data;
    p->startingTransparency = startingTransparency;
    p->endTransparency = endTransparency;
}



void particleUpdate(Entity* this){
    Particle* data = this->data;

    // update movement
    {
        if (!data->isStatic){
            this->x += data->xVelocity;
            this->y += data->yVelocity;
            data->yVelocity += data->gravity;

        }
    }

    // animation
    {
        if (data->isAnimated){
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
        float lifePercentage = 1.0f - ((float)data->lifeTime / data->startingLifeTime);
        Color c = {255, 255, 255, (unsigned char)lerp(data->startingTransparency, data->endTransparency, lifePercentage)};
        drawFC(data->currentSprite + data->startFrame, this->x, this->y, data->isFlipped, c, LAYER_EFFECTS);
    }
}


void flipParticle(Entity* particle){
    ((Particle*) (particle->data))->isFlipped = true;
}


void particleOnCollide(Entity* this, Entity* other){
    
}

void particleOnDestroy(Entity* this){

}

void particleClean(Entity* this){
    
}