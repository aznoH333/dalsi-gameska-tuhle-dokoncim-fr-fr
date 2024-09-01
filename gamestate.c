#include "gamestate.h"
#include "gameCamera.h"
#include "gameprogress.h"


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
            startLevel(gamestate->gameplay, "./gamedata/editor/1.lvl");
            break;
    }
}



GameState* initGameState(){
    GameState* out = malloc(sizeof(GameState));

    out->currentState = GAME_STATE_EDITOR;
    out->gameplay = getGameplay();
    out->editor = getLevelEditor();
    loadCurrentState(out);

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
        changeGameState(gamestate, 1);
    }
    if (IsKeyPressed(KEY_THREE)){
        changeGameState(gamestate, 2);
    }
    
    
    switch (gamestate->currentState) {
        case GAME_STATE_MAIN_MENU:
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