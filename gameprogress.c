#include "gameprogress.h"
#include "stdlib.h"
#include "spritedata.h"
#include "gutil.h"
#include "particleEffect.h"

GameProgress* gameProgressInstance;

GameProgress* initGameProgress(){
    GameProgress* output = malloc(sizeof(GameProgress));
    output->score = 0;
    return output;
}


GameProgress* getGameProgress(){
    if (!gameProgressInstance) gameProgressInstance = initGameProgress();
    return gameProgressInstance;
}


void unloadGameProgress(){
    free(gameProgressInstance);
}

void setupParticleValues(int ammount, int* zeroCount, int* numeralSprite){
    switch (ammount) {
        case 100: *zeroCount = 1; *numeralSprite = SPRITE_START_EFFECTS + 29; break;
        case 200: *zeroCount = 1; *numeralSprite = SPRITE_START_EFFECTS + 30; break;
        case 500: *zeroCount = 1; *numeralSprite = SPRITE_START_EFFECTS + 31; break;
        case 1000: *zeroCount = 1; *numeralSprite = SPRITE_START_EFFECTS + 32; break;
        case 2000: *zeroCount = 1; *numeralSprite = SPRITE_START_EFFECTS + 33; break;
        case 5000: *zeroCount = 1; *numeralSprite = SPRITE_START_EFFECTS + 34; break;
        case 10000: *zeroCount = 2; *numeralSprite = SPRITE_START_EFFECTS + 29; break;

        default: gLog(LOG_ERR, "Unknown score value [%d]", ammount); break;
    }
}

void addScore(int x, int y, int ammount){
    
    // add score
    getGameProgress()->score += ammount;



    // setup vars
    int zeroCount = 0;
    int numeralSprite = 0;
    setupParticleValues(ammount, &zeroCount, &numeralSprite);

    // spawn particles
    EntityManager* m = getEntityManager();
    
    for (int i = 0; i < zeroCount + 1; i++){
        Entity* p = initStaticParticle(x + (i * 8), y, i == 0 ? numeralSprite : SPRITE_START_EFFECTS + 28, 60);
        makeParticleMove(p, 0.0f, -0.5f, 0.0f);
        makeParticleChangeTransparency(p, 255, 0);
        addEntity(m, p);
    }
    
}
