#include "levelScripts.h"
#include "gameCamera.h"
#include "gutil.h"


void l5s0(){
    setScrollSpeed(getCameraManager(), 1.5f);
    gLog(LOG_DBG, "i am");
}

#define MAX_SCRIPTS_IN_LEVEL 5
#define SHITTY_HARDCODED_LEVEL_INDEX 4
void (*(scriptMap[][MAX_SCRIPTS_IN_LEVEL]))() = {
    {},
    {},
    {},
    {},
    {
        l5s0
    },{},
};


int currentLevelIndex;
int currentScriptIndex = 0;
void setScriptData(int levelIndex){
    if (levelIndex == -1) levelIndex = SHITTY_HARDCODED_LEVEL_INDEX;
    currentLevelIndex = levelIndex;
    currentScriptIndex = 0;
}



void activateScript(){
    gLog(LOG_DBG, "activating script level[%d] scriptId[%d]", currentLevelIndex, currentScriptIndex);

    scriptMap[currentLevelIndex][currentScriptIndex]();

    currentScriptIndex  += currentScriptIndex < MAX_SCRIPTS_IN_LEVEL - 1;
}