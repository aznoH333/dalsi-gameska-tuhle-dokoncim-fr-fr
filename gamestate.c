#include "gamestate.h"
#include "gameCamera.h"
#include "gameprogress.h"
#include "mainMenu.h"
#include <stdio.h>



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
    }


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
    gLog(LOG_INF, "starting level %s", convertLevelIndexToFilePath(this->nextLevel));
    changeGameState(this, GAME_STATE_GAME);

}

void setNextLevelIndex(GameState* this, int index){
    gLog(LOG_INF, "changed level index [%d]", index);
    
    this->nextLevel = index;
}