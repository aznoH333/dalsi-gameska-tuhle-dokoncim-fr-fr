#include "levelScreen.h"
#include "gframework.h"
#include "gamestate.h"
#include "gutil.h"

int sLevelNumber = 0;
char sLevelName[20];
int sTransitionTimer = 0;
#define DEFAULT_TRANSITION_LENGTH 120

void setLevelScreen(int levelNumber, char* levelName){
    gLog(LOG_INF,"Level screen updated [%d] [%s]", levelNumber, levelName);
    copyCharArray(levelName, sLevelName, 20);
    sLevelNumber = levelNumber;
    sTransitionTimer = DEFAULT_TRANSITION_LENGTH;
}

#define LEVEL_NUMBER_X (SCREEN_WIDTH / 2) - 100
#define LEVEL_NUMBER_Y (SCREEN_HEIGHT / 2) - 16


#define LEVEL_NAME_X (SCREEN_WIDTH / 2) - 100
#define LEVEL_NAME_Y (SCREEN_HEIGHT / 2) + 16


void updateLevelScreen(){
    textF("level %d", LEVEL_NUMBER_X, LEVEL_NUMBER_Y, sLevelNumber);
    textF("%s", LEVEL_NAME_X, LEVEL_NAME_Y, sLevelName);
    if (sTransitionTimer > 0){
        sTransitionTimer--;
    }else {
        activateScreenTransition(getGameState(), GAME_STATE_GAME);
        //changeGameState(getGameState(), GAME_STATE_GAME);
    }
}