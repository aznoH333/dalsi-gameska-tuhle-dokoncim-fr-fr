#include "gameplay.h"
#include "entities.h"
#include "gameCamera.h"
#include "level.h"
#include "player.h"
#include "gameprogress.h"
#include "enemy.h"

Gameplay* initGameplay(){
    Gameplay* output = malloc(sizeof(Gameplay));
    output->level = 0;
    output->playerX = 0.0f;
    output->playerY = 0.0f;
    output->hasLoadedLevel = false;
    output->currentPassiveMarkerEffect = MARKER_EFFECT_NONE;
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
    return isInRange(cameraMarkerId, 2, 3) || isInRange(cameraMarkerId, 23, 28);
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
        case 23: return MARKER_EFFECT_STOP;
        case 24: return MARKER_EFFECT_FLY_GREY;
        case 25: return MARKER_EFFECT_FLY_RED;
        case 26: return MARKER_EFFECT_FLY_BLUE;
        case 27: return MARKER_EFFECT_CUSTOM_SCRIPT;
        default: gLog(LOG_ERR, "Unrecognized script id [%d]", markerId); return 0;
    }
}

void startLevel(Gameplay* g, const char* levelPath){
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
        addEntity(getEntityManager(), initPlayer(playerMarker->x * 16, playerMarker->y * 16));
    }


    // play music
    {
        playMusic(0);
    }
}

void setPlayerCoordinates(Gameplay* gameplay, float x, float y){
    gameplay->playerX = x;
    gameplay->playerY = y;
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
        unloadLevel(g->level);
        clearCameraMarkers(getCameraManager());
        removeAllEntities(getEntityManager());
        g->hasLoadedLevel = false;
    }
}

void disposeGameplay(Gameplay* g){
    unloadGameplay(g);
    free(g);
}

void setMarkerEffect(int markerEffect){
    if (markerEffect == MARKER_EFFECT_NONE){
        return;
    }

    if (markerEffect == MARKER_EFFECT_STOP){
        getGameplay()->currentPassiveMarkerEffect = MARKER_EFFECT_NONE;
    }else {
        getGameplay()->currentPassiveMarkerEffect = markerEffect;
    }

    
}


void doFlyEffect(Gameplay* this, int flyType){
    if (getGlobalTimer() % 80 == 0){
        // get camera position
        CameraManager* camera = getCameraManager();

        int x = camera->cameraX;
        int y = camera->cameraY;

        // pick x
        x += getRandomInt(DEFAULT_GAME_WIDTH);
        if (randomChance(0.5f)){
            y += DEFAULT_GAME_HEIGHT;
        }

        addEntity(getEntityManager(), initEnemy(x, y, ENEMY_GREY_FLY + flyType));
    }
}

void updateScripts(Gameplay* this){
    switch (this->currentPassiveMarkerEffect) {
        default:
        case MARKER_EFFECT_NONE: return;
        case MARKER_EFFECT_FLY_GREY:
        case MARKER_EFFECT_FLY_RED:
        case MARKER_EFFECT_FLY_BLUE:
            doFlyEffect(this, this->currentPassiveMarkerEffect - 1);
            return;
        case MARKER_EFFECT_CUSTOM_SCRIPT:
            gLog(LOG_ERR, "Not yet implemented");
            return;        
    }
}



void updateGameplay(Gameplay* g){
    drawLevel(g->level, LEVEL_DRAW_GAME);
    displayPlayerUi();
    updateScripts(g);
    updateEntityManager(getEntityManager());
    updateCameraManager(getCameraManager());
}
