#include "gameplay.h"
#include "entities.h"
#include "gameCamera.h"
#include "gframework.h"
#include "gutil.h"
#include "level.h"
#include "player.h"
#include "gameprogress.h"
#include "enemy.h"
#include "spritedata.h"
#include "gamestate.h"
#include "levelScripts.h"
#include "bossSpawner.h"
#include "particleEffect.h"

Gameplay* initGameplay(){
    Gameplay* output = malloc(sizeof(Gameplay));
    output->level = 0;
    output->playerX = 0.0f;
    output->playerY = 0.0f;
    output->playerRespawnX = 0.0f;
    output->playerRespawnY = 0.0f;
    output->hasLoadedLevel = false;
    output->currentPassiveMarkerEffect = MARKER_EFFECT_NONE;
    output->startWaterHeight = -1;
    output->targetWaterHeight = -1;
    output->waterProgress = 0.0f;
    output->respawnCount = DEFAULT_PLAYER_HP;
    output->respawnTimer = 0;
    output->canTriggerScript = true;
    output->bossSpawnTimer = 0;
    return output;    
}


Gameplay* gameplayInstance;
Gameplay* getGameplay(){
    if (gameplayInstance == 0) {
        gameplayInstance = initGameplay();
    }
    return gameplayInstance;
}

bool isCameraMarker(int cameraMarkerId){
    return isInRange(cameraMarkerId, 2, 3) || isInRange(cameraMarkerId, 21, 24);
}

int getMarkerType(int markerId){
    if (markerId <= 3){
        return markerId - 2;
    }
    return CAMERA_SPECIAL;
}

int getMarkerEffect(int markerId){
    switch (markerId) {
        case 2:
        case 3: return MARKER_EFFECT_NONE;
        case 21: return MARKER_EFFECT_STOP;
        case 22: return MARKER_EFFECT_FLY_RED;
        case 23: return MARKER_EFFECT_FLY_BLUE;
        case 24: return MARKER_EFFECT_CUSTOM_SCRIPT;
        default: gLog(LOG_ERR, "Unrecognized script id [%d]", markerId); return 0;
    }
}

void startLevel(Gameplay* g, const char* levelPath){
    
    resetWater();
    // reset hp
    g->respawnCount = DEFAULT_PLAYER_HP;
    g->playerRespawnX = -99999;
    g->playerRespawnY = 0;


    // level load
    {
        if (g->hasLoadedLevel){
            unloadLevel(g->level);
        }
            
        
        g->level = loadLevel(levelPath);
        g->hasLoadedLevel = true;
    }

    // setup camera markers
    {
        CameraManager* m = getCameraManager();
        int markerCount = 0;

        for (int x = 0; x < g->level->width; x++){
            for (int y = 0; y < g->level->height; y++){
                for (int i = 0; i < g->level->entityeMarkers->elementCount; i++){
                    EntityMarker* marker = vectorGet(g->level->entityeMarkers, i);

                    if (marker->x == x && marker->y == y && isCameraMarker(marker->id)){
                        addCameraMarker(m, x * 16, y * 16, getMarkerType(marker->id), getMarkerEffect(marker->id));
                        markerCount++;
                    }
                }
            }
        }

        if (markerCount < 2){
            gLog(LOG_WAR, "Too few camera markers in %s", levelPath);
        }
    }


    // find player
    {
        EntityMarker* playerMarker;
        for (int i = 0; i < g->level->entityeMarkers->elementCount; i++){
            playerMarker = vectorGet(g->level->entityeMarkers, i);
            
            if (playerMarker->id == 0){
                break;
            }
        }
        updateGameCameraPosition(getCameraManager(), playerMarker->x * 16, playerMarker->y * 16);
        addEntity(getEntityManager(), initPlayer(playerMarker->x * 16, playerMarker->y * 16, 0));
    }


    // play music
    {
        playMusic(g->level->songId);
    }

    // reset scripts
    {
        g->currentPassiveMarkerEffect = MARKER_EFFECT_NONE;
    }
}

void touchedLevelExit(Gameplay* this){
    
    if (strEquals(this->level->levelFile->filePath, "./gamedata/editor/1.lvl")){
        
        // return to editor
        changeGameState(getGameState(), GAME_STATE_EDITOR);
    
    }else {

        // load next level
        goToNextLevel(getGameState());

    }
}



void resetWater(){
    Gameplay* g = getGameplay();
    // reset water
    g->targetWaterHeight = -1;
    g->startWaterHeight = -1;
    g->waterProgress = 0;
}

void setPlayerCoordinates(Gameplay* gameplay, float x, float y, bool isGrounded){
    gameplay->playerX = x;
    gameplay->playerY = y;
    if (isGrounded && pythagoras(gameplay->playerRespawnX, gameplay->playerRespawnY, x, y) > 100.0f){
        gameplay->playerRespawnX = x;
        gameplay->playerRespawnY = y;
    }
}


EntityMarker* getCollidingMarker(Gameplay* g, Entity* entity){

    for (int i = 0; i < g->level->entityeMarkers->elementCount; i++){
        EntityMarker* marker = vectorGet(g->level->entityeMarkers, i);

        if (checkBoxCollisions(marker->x * 16, marker->y * 16, 16, 16, entity->x, entity->y, entity->w, entity->h)){
            return marker;
        }
    }

    return 0;
}


void unloadGameplay(Gameplay* g){
    if (g->hasLoadedLevel){
        resetCameraManager(getCameraManager());
        removeAllEntities(getEntityManager());
        g->hasLoadedLevel = false;
        unloadLevel(g->level);
    }
}

void disposeGameplay(Gameplay* g){
    unloadGameplay(g);
    free(g);
}
//-------------------------------------------------------
// Water
//-------------------------------------------------------

void setMarkerEffect(int markerEffect){
    if (markerEffect == MARKER_EFFECT_NONE){
        return;
    }

    if (markerEffect == MARKER_EFFECT_STOP){
        getGameplay()->currentPassiveMarkerEffect = MARKER_EFFECT_NONE;
    }else {
        getGameplay()->currentPassiveMarkerEffect = markerEffect;
        getGameplay()->canTriggerScript = true;
    }

    
}

bool isFlySpawnValid(Gameplay* this, int x, int y){
    float distance = pythagoras(this->playerX, this->playerY, x, y);

    return distance > 128;
}


void doFlyEffect(Gameplay* this, int flyType){
    if (getGlobalTimer() % 110 == 0){
        // get camera position
        CameraManager* camera = getCameraManager();

        int x = camera->cameraX;
        int y = camera->cameraY;

        int attemtps = 5;
        do {
            // pick x
            x += getRandomInt(DEFAULT_GAME_WIDTH);
            if (randomChance(0.5f)){
                y += DEFAULT_GAME_HEIGHT;
            }
        } while (!isFlySpawnValid(this, x, y) && attemtps-- > 0);


        addEntity(getEntityManager(), initEnemy(x, y, ENEMY_FLY_RED + flyType));
    }
}

void updateScripts(Gameplay* this){
    
    switch (this->currentPassiveMarkerEffect) {
        default:
        case MARKER_EFFECT_NONE: return;
        case MARKER_EFFECT_FLY_RED:
        case MARKER_EFFECT_FLY_BLUE:
            doFlyEffect(this, this->currentPassiveMarkerEffect - 1);
            return;
        case MARKER_EFFECT_CUSTOM_SCRIPT:
            if (this->canTriggerScript){
                activateScript();
                this->canTriggerScript = false;
            }

            return;        
    }
}
//-------------------------------------------------------
// Water
//-------------------------------------------------------
void setWaterHeight(int height){
    Gameplay* g = getGameplay();
    
    if (g->targetWaterHeight == -1){// first set
        g->targetWaterHeight = height;
        g->startWaterHeight = height;
        g->waterProgress = 1.0f;
    }else {// normal set
        g->startWaterHeight = g->targetWaterHeight;
        g->targetWaterHeight = height;
        g->waterProgress = 0.0f;
    }
    
}

const Color WATER_COLOR = {255, 255, 255, 155};
void updateWater(Gameplay* this){
    if (this->targetWaterHeight == -1){
        return;
    }

    // calculate y
    float waterHeight = lerp(this->startWaterHeight, this->targetWaterHeight,this->waterProgress) + (sin(getGlobalTimer() * 0.02f) * 5.0f);
    if (this->waterProgress < 1.0f){
        this->waterProgress += 0.01f;
    }else {
        this->waterProgress = 1.0f;
    }
    
    // calculate x
    float x = getCameraManager()->cameraX - (getGlobalTimer() % 16);

    // draw water
    float maxCameraY = getCameraManager()->cameraY + DEFAULT_GAME_HEIGHT;
    for (int i = 0; i < DEFAULT_GAME_WIDTH + 16; i += 16){
        drawC(SPRITE_START_WATER + this->level->waterType, i + x, waterHeight, WATER_COLOR, LAYER_EFFECTS);
        // draw rest of water
        for (float j = waterHeight + 16.0f; j < maxCameraY; j += 16){
            drawC(SPRITE_START_WATER + 3 + this->level->waterType, i + x, j, WATER_COLOR, LAYER_EFFECTS);
        }
    }
}
#define PLAYER_RESPAWN_INVINCIBILITY 140
#define PLAYER_RESPAWN_TIMER 30
//-------------------------------------------------------
// Update
//-------------------------------------------------------
void updateBossSpawning(Gameplay* this);
void updateGameplay(Gameplay* g){
    drawLevel(g->level, LEVEL_DRAW_GAME);
    displayPlayerUi();
    updateScripts(g);
    updateEntityManager(getEntityManager());
    updateCameraManager(getCameraManager());
    updateWater(g);
    updateBossSpawning(g);

    // respawn
    g->respawnTimer -= g->respawnTimer >= 0;
    if (g->respawnTimer == 0){
        int rX = g->playerRespawnX;
        int rY = g->playerRespawnY;

        CameraManager* cam = getCameraManager();
        // set fallback coordinates if respawn is too far
        if (!isOnScreen(cam, rX, rY, 16, 16)){
            rX = cam->cameraX + 64;
            rY = cam->cameraY + 64;
        }
        
        addEntity(getEntityManager(), initPlayer(rX, rY, PLAYER_RESPAWN_INVINCIBILITY));
    }
    
}

//-------------------------------------------------------
// Respawning
//-------------------------------------------------------
void playerJustDied(Gameplay* this){
    if (this->respawnCount > 0){
        this->respawnCount--;
        // respawn player
        this->respawnTimer = PLAYER_RESPAWN_TIMER;
        //addEntity(getEntityManager(), initPlayer(this->playerRespawnX, this->playerRespawnY, PLAYER_RESPAWN_INVINCIBILITY));
    }else {
        activateScreenTransition(getGameState(), GAME_STATE_GAME_OVER);
    }
}

//-------------------------------------------------------
// Boss spawner
//-------------------------------------------------------
#define BOSS_SPAWN_TIME 180
#define BOSS_SHOWUP 60
void registerBossSpawner(Gameplay* this, Entity* spawner){
    this->bossSpawnerPtr = spawner;
}
void spawnBoss(Gameplay* this){
    fadeMusicAway();
    this->bossSpawnTimer = BOSS_SPAWN_TIME;
}


void initBossParticle(Gameplay* this, int sprite, int x, int y){
    Entity* p = initStaticParticle(this->bossSpawnerPtr->x + x, this->bossSpawnerPtr->y + y, sprite, BOSS_SHOWUP);
    makeParticleChangeTransparency(p, 0, 255);
    addEntity(getEntityManager(), p);
}

void initBossParticles(Gameplay* this){
    int baseSprite = SPRITE_START_ENTITIES + 33 + ((((BossSpawner*)(this->bossSpawnerPtr->data))->bossType == 1) * 7);
    initBossParticle(this, baseSprite + 2, -8, 0);
    initBossParticle(this, baseSprite + 3, 8, 0);
    // head
    initBossParticle(this, baseSprite, -1, -11);
}

void updateBossSpawning(Gameplay* this){
    this->bossSpawnTimer -= this->bossSpawnTimer > 0;


    if (this->bossSpawnTimer == BOSS_SHOWUP){
        // spawn fade particle
        initBossParticles(this);
        playMusic(6);
    }else if (this->bossSpawnTimer == 1){
        // spawn boss
        addEntity(getEntityManager(), initEnemy(this->bossSpawnerPtr->x, this->bossSpawnerPtr->y, ((BossSpawner*)(this->bossSpawnerPtr->data))->bossType + ENEMY_BOSS_RED));
        this->bossSpawnerPtr->destroyFlag = DESTROY_SILENT;
    }
}
