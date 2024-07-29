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
            unloadGameplay(gamestate->gameplay);
            break;
    }
}


void loadCurrentState(GameState* gamestate){
    switch (gamestate->currentState) {
        case GAME_STATE_MAIN_MENU:
            break;
        case GAME_STATE_EDITOR:
            loadLevelEditorLevel(gamestate->editor, "./gamedata/1.lvl");
            break;
        case GAME_STATE_GAME:
            startLevel(gamestate->gameplay, "./gamedata/1.lvl");
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



void changeGameState(GameState* gamestate, int newState){
    unloadCurrentState(gamestate);
    gamestate->currentState = newState;
    setCameraZoom(DEFAULT_CAMERA_ZOOM);
    loadCurrentState(gamestate);
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
}