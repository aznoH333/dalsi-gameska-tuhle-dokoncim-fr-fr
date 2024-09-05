#include "gamestate.h"
#include "gameCamera.h"
#include "gameprogress.h"
#include "mainMenu.h"
#include "levelScreen.h"
#include <stdio.h>


//--------------------------------------------------
// screen transitions
//--------------------------------------------------

#define SCREEN_TRANSITION_PIXEL_SIZE DEFAULT_CAMERA_ZOOM
#define SCREEN_TRANSITION_WIDTH DEFAULT_GAME_WIDTH / 16 + 1
#define SCREEN_TRANSITION_HEIGHT DEFAULT_GAME_HEIGHT /16 + 1

void updateScreenTransition(GameState* this){
    if (this->screenTransition == 0){
        return;
    }

    this->screenTransition--;


    float transitionPercentage = ((float)this->screenTransition / (float)SCREEN_TRANSITION_LENGTH) * 2.6f - 1.3f;
    // draw
    for (int x = 0; x < SCREEN_TRANSITION_WIDTH; x++){
        
        // calculate sprite
        int spriteIndex = 0;
        float xPrecentage = ((float)x / (float)(SCREEN_TRANSITION_WIDTH));
        float distToNearestEgde = xPrecentage - transitionPercentage;


        if (xPrecentage < transitionPercentage - 0.15f || xPrecentage > transitionPercentage + 1.15f){
            continue;
        // hardcoded garbage
        }else if (distToNearestEgde < -0.15f || distToNearestEgde > 1.15f){
            spriteIndex = 3;
        }
        else if (distToNearestEgde < -0.10f || distToNearestEgde > 1.10f){
            spriteIndex = 2;
        }else if (distToNearestEgde < -0.05f || distToNearestEgde > 1.05f){
            spriteIndex = 1;
        }


        // draw collumn
        for (int y = 0; y < SCREEN_TRANSITION_HEIGHT; y++){
            drawS(SPRITE_START_EFFECTS + 35 + spriteIndex, x * 16 * SCREEN_TRANSITION_PIXEL_SIZE, y * 16 * SCREEN_TRANSITION_PIXEL_SIZE, SCREEN_TRANSITION_PIXEL_SIZE, LAYER_STATIC_UI);
        }
    }

    // do transition
    if (this->screenTransition == SCREEN_TRANSITION_LENGTH >> 1){
        changeGameState(this, this->screenTransitionTarget);
        this->screenTransitionTarget = -1;

    }
}


void activateScreenTransition(GameState* this, int newState){
    if (this->screenTransition == 0){
        this->screenTransition = SCREEN_TRANSITION_LENGTH;
        this->screenTransitionTarget = newState;
    }

}


//--------------------------------------------------
// gamestate functions
//--------------------------------------------------
void unloadCurrentState(GameState* gamestate){
    switch (gamestate->currentState) {
        case GAME_STATE_MAIN_MENU:
            break;
        case GAME_STATE_EDITOR:
            unloadLevelEditor(gamestate->editor);
            break;
        case GAME_STATE_GAME:
            ShowCursor();
            unloadGameplay(gamestate->gameplay);
            break;
        case GAME_STATE_LEVEL_SCREEN:
            break;
    }
}

// -1 editor level
// other == /gamedata/levels/[index].lvl
#define SHITTY_WORKAROUND_START 18
char shittyWorkaround[] = "./gamedata/levels/00.lvl";
char* convertLevelIndexToFilePath(int index){
    if (index == -1){
        return "./gamedata/editor/1.lvl";
    }else {
        shittyWorkaround[SHITTY_WORKAROUND_START] = '0' + (index - (index%10)) / 10;
        shittyWorkaround[SHITTY_WORKAROUND_START + 1] = '0' + (index%10);

        return shittyWorkaround;
    }
}


void loadCurrentState(GameState* gamestate){
    switch (gamestate->currentState) {
        case GAME_STATE_MAIN_MENU:
            break;
        case GAME_STATE_EDITOR:
            loadLevelEditorLevel(gamestate->editor, "./gamedata/editor/1.lvl");
            break;
        case GAME_STATE_GAME:
            HideCursor();
            startLevel(gamestate->gameplay, convertLevelIndexToFilePath(gamestate->nextLevel));
            break;
        case GAME_STATE_LEVEL_SCREEN:
            break;
    }
}



GameState* initGameState(){
    GameState* out = malloc(sizeof(GameState));

    out->currentState = GAME_STATE_EDITOR;
    out->gameplay = getGameplay();
    out->editor = getLevelEditor();
    out->screenTransition = 0;
    out->screenTransitionTarget = -1;
    loadCurrentState(out);
    out->nextLevel = 0;

    return out;
}

GameState* gamestateInstance;
GameState* getGameState(){
    if (gamestateInstance == 0){
        gamestateInstance = initGameState();
    }
    return gamestateInstance;
}


int pendingGameState = -1;
void changeGameState(GameState* gamestate, int newState){
    pendingGameState = newState;
}

void updateGameStateChange(GameState* this){

    unloadCurrentState(this);
    this->currentState = pendingGameState;
    setCameraZoom(DEFAULT_CAMERA_ZOOM);
    loadCurrentState(this);

    pendingGameState = -1;
}


void disposeGameState(GameState* gamestate){
    disposeLevelEditor(gamestate->editor);
    disposeGameplay(gamestate->gameplay);
    unloadGameProgress();
    unloadEntityManager(getEntityManager());
    unloadCameraManager(getCameraManager());
}




void updateGameState(GameState* gamestate){

    if (IsKeyPressed(KEY_ONE)){
        changeGameState(gamestate, 0);
    }
    if (IsKeyPressed(KEY_TWO)){
        setNextLevelIndex(gamestate, -1);
        changeGameState(gamestate, 1);
    }
    if (IsKeyPressed(KEY_THREE)){
        changeGameState(gamestate, 2);
    }

    if (IsKeyPressed(KEY_FOUR)){
        gamestate->screenTransition = 100;
    }
    
    
    switch (gamestate->currentState) {
        case GAME_STATE_MAIN_MENU:
            updateMainMenu();
            break;
        case GAME_STATE_EDITOR:
            updateLevelEditor(gamestate->editor);
            break;
        case GAME_STATE_GAME:
            updateGameplay(gamestate->gameplay);
            break;
        case GAME_STATE_LEVEL_SCREEN:
            updateLevelScreen();
            break;
    }

    updateScreenTransition(gamestate);



    if (pendingGameState != -1){
        updateGameStateChange(gamestate);
    }

    
}


//--------------------------------------------------
// Level functions
//--------------------------------------------------
void goToNextLevel(GameState* this){
    this->nextLevel += this->nextLevel >= 0; // dont increment -1
    startCurrentLevel(this);
}

void startCurrentLevel(GameState* this){
    
    char* a = convertLevelIndexToFilePath(this->nextLevel);
    gLog(LOG_DBG, "starting level %s", a);
    //changeGameState(this, GAME_STATE_GAME);
    Level* temp = loadLevel(a);
    setLevelScreen(this->nextLevel + 1, temp->name);
    unloadLevel(temp);
    activateScreenTransition(this, GAME_STATE_LEVEL_SCREEN);
    //changeGameState(this, GAME_STATE_LEVEL_SCREEN);
}

void setNextLevelIndex(GameState* this, int index){
    gLog(LOG_DBG, "changed level index [%d]", index);
    
    this->nextLevel = index;
}