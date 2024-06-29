#ifndef PARTICLE
#define PARTICLE

#include "entities.h"

struct Particle{
    bool isStatic;
    bool isAnimated;
    float gravity;
    float xVelocity;
    float yVelocity;
    int lifeTime;
    int frameDuration;
    int startFrame;
    int endFrame;
    int timer;
    int currentSprite;
};
typedef struct Particle Particle;


Entity* initStaticParticle(int x, int y, int sprite, int lifeTime);

void particleUpdate(Entity* this);
void particleOnCollide(Entity* this, Entity* other);
void particleOnDestroy(Entity* this);
void particleClean(Entity* this);

void makeParticleAnimated(Entity* particle, int endFrame, int frameDuration);
void makeParticleMove(Entity* particle, float xVelocity, float yVelocity, float gravity);
void makeParticleAnimatedSingleLoop(Entity* particle, int endFrame);

#endif