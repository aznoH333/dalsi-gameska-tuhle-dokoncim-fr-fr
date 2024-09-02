#include "levelScreen.h"
#include "gframework.h"

int sLevelNumber = 0;
char* sLevelName = 0;

void setLevelScreen(int levelNumber, char* levelName){
    sLevelName = levelName;
    sLevelNumber = levelNumber;
}

#define LEVEL_NUMBER_X DEFAULT_GAME_WIDTH / 2 - 100
#define LEVEL_NUMBER_Y DEFAULT_GAME_HEIGHT / 2 - 16


#define LEVEL_NAME_X DEFAULT_GAME_WIDTH / 2 - 100
#define LEVEL_NAME_Y DEFAULT_GAME_HEIGHT / 2 + 16


void updateLevelScreen(){
    textF("level %d", LEVEL_NUMBER_X, LEVEL_NUMBER_Y, sLevelNumber);
    textF("%s", LEVEL_NAME_X, LEVEL_NAME_Y, sLevelName);
}