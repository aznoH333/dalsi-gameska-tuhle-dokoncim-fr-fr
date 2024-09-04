#include "gameprogress.h"
#include "stdlib.h"
#include "spritedata.h"
#include "gutil.h"
#include "particleEffect.h"
#include "gframework.h"
#include "gameplay.h"
#include "spritedata.h"

GameProgress* gameProgressInstance;

GameProgress* initGameProgress(){
    GameProgress* output = malloc(sizeof(GameProgress));
    output->score = 0;
    output->scoreSizeMultiplier = 0;
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

const int MAX_SCORE_SIZE_MULTIPLIER = 10;

void addScore(int x, int y, int ammount){
    
    // add score
    getGameProgress()->score += ammount;
    getGameProgress()->scoreSizeMultiplier = MAX_SCORE_SIZE_MULTIPLIER;


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

#define HEALTH_DISPLAY_X 10
#define HEALTH_DISPLAY_Y 10
#define HEALTH_DISPLAY_GAP 16

void displayPlayerUi(){
    GameProgress* g = getGameProgress();
    g->scoreSizeMultiplier -= g->scoreSizeMultiplier > 0;


    // score display
    float multiplierPercentage = ((float)g->scoreSizeMultiplier / MAX_SCORE_SIZE_MULTIPLIER);
    // i have no clue why but an offset of 20 produces the correct results
    drawTextF("score %010d", 1000 - (20 * multiplierPercentage), 8, 2.0f + (0.3f * multiplierPercentage), WHITE, LAYER_STATIC_UI, g->score);


    // health display
    int hp = getGameplay()->respawnCount;
    for (int i = 0; i < hp; i++){
        drawS(SPRITE_START_EFFECTS + 27, HEALTH_DISPLAY_X + (i * HEALTH_DISPLAY_GAP), HEALTH_DISPLAY_Y, 2.0f, LAYER_STATIC_UI);
    }
}

