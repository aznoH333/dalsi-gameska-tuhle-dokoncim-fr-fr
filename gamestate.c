#include "gamestate.h"
#include "gameCamera.h"



void unloadCurrentState(GameState* gamestate){
    switch (gamestate->currentState) {
        case GAME_STATE_MAIN_MENU:
            break;
        case GAME_STATE_EDITOR:
            break;
        case GAME_STATE_GAME:
            
            break;
    }
}


void loadCurrentState(GameState* gamestate){
    switch (gamestate->currentState) {
        case GAME_STATE_MAIN_MENU:
            break;
        case GAME_STATE_EDITOR:
            break;
        case GAME_STATE_GAME:
            startLevel(gamestate->gameplay, "./gamedata/1.lvl");
            break;
    }
}



GameState* initGameState(){
    GameState* out = malloc(sizeof(GameState));

    out->currentState = GAME_STATE_GAME;
    out->gameplay = getGameplay();
    //out->editor = getLevelEditor();
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
    loadCurrentState(gamestate);
}



void disposeGameState(GameState* gamestate){
    //unloadLevelEditor(gamestate->editor);
    unloadGameplay(gamestate->gameplay);
    unloadEntityManager(getEntityManager());
    unloadCameraManager(getCameraManager());
}

void updateGameState(GameState* gamestate){
    switch (gamestate->currentState) {
        case GAME_STATE_MAIN_MENU:
            break;
        case GAME_STATE_EDITOR:
            break;
        case GAME_STATE_GAME:
            updateGameplay(gamestate->gameplay);
            break;
    }
}