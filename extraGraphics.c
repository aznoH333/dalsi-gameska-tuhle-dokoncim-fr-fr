#include "extraGraphics.h"
#include "particleEffect.h"
#include "spritedata.h"

void genericDeathFunction(Entity* this){
    
    ExtraGraphics* data = this->data;

    int explosionRadius = 5 + ((data->lifeTime > 10) * 5);

    data->lifeTime--;

    if (data->lifeTime % 2 == 0){
        // spawn explosion
        int spreadX = getRandomIntR(-explosionRadius, explosionRadius);
        int spreadY = getRandomIntR(-explosionRadius, explosionRadius);

        Entity* p = initStaticParticle(this->x + spreadX, this->y + spreadY, SPRITE_START_EFFECTS + 7, 15);
        makeParticleAnimatedSingleLoop(p, SPRITE_START_EFFECTS + 10);
        addEntity(getEntityManager(), p);
    }

    if (data->lifeTime <= 0){
        this->shouldDestroy = true;
    }
}


void (*initGraphicData(ExtraGraphics* data))(Entity*){
    switch (data->type) {
        case GRAPHICS_DEATH_SMALL:
            data->lifeTime = 10;
            return &genericDeathFunction;
        case GRAPHICS_DEATH_LARGE:
            data->lifeTime = 20;
            return &genericDeathFunction;
        case GRAPHICS_DEATH_ROBOT:
            data->lifeTime = 12;
            return &genericDeathFunction;
        case GRAPHICS_DEATH_LARGE_ROBOT:
            data->lifeTime = 23;
            return &genericDeathFunction;
    }
    gLog(LOG_ERR, "Invalid graphics id [%d]", data->type);
    return 0;
}

Entity* initExtraGraphic(int x, int y, int type){
    ExtraGraphics* data = malloc(sizeof(ExtraGraphics*));
    data->type = type;


    void (*update)(struct Entity *) = initGraphicData(data);
    
    return initEntity(x, y, 0, 0, ENTITY_OTHER, data, update, &graphicOnCollide, &graphicOnDestroy, &graphicClean);
}
void graphicOnCollide(Entity* data, Entity* other){}
void graphicOnDestroy(Entity* data){}
void graphicClean(Entity* data){}