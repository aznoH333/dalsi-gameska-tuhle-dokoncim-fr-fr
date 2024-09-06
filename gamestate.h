#ifndef GAME_STATE
#define GAME_STATE


#include "gameplay.h"
#include "levelEditor.h"


#define GAME_STATE_MAIN_MENU 0
#define GAME_STATE_GAME 1
#define GAME_STATE_EDITOR 2
#define GAME_STATE_LEVEL_SCREEN 3
#define GAME_STATE_GAME_OVER 4

#define SCREEN_TRANSITION_LENGTH 100
struct GameState{
    int currentState;
    Gameplay* gameplay;
    LevelEditor* editor;
    int nextLevel;
    int screenTransition;
    int screenTransitionTarget;
};
typedef struct GameState GameState;


// game state functions
GameState* getGameState();
void changeGameState(GameState* gamestate, int newState);
void disposeGameState(GameState* gamestate);
void updateGameState(GameState* gamestate);
void activateScreenTransition(GameState* this, int newState);

// level functions
void goToNextLevel(GameState* gamestate);
void startCurrentLevel(GameState* gamestate);
void setNextLevelIndex(GameState* gamestate, int index);



#endif