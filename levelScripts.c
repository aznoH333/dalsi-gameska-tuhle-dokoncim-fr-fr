#include "levelScripts.h"
#include "gameCamera.h"
#include "gutil.h"
#include "gameplay.h"

//---------------------------------------------
// Level 5
//---------------------------------------------
void l5s0(){
    setScrollSpeed(getCameraManager(), 1.5f);
    gLog(LOG_DBG, "i am");
}


//---------------------------------------------
// Level 6
//---------------------------------------------
void l6s0(){
    spawnBoss(getGameplay());
}

//---------------------------------------------
// Level 8
//---------------------------------------------
void l8s0(){
	setScrollSpeed(getCameraManager(), 1.3f);
}
//---------------------------------------------
// Level 10
//---------------------------------------------
void l10s0(){
	spawnBoss(getGameplay());
}

#define MAX_SCRIPTS_IN_LEVEL 5
void (*(scriptMap[][MAX_SCRIPTS_IN_LEVEL]))() = {
    {},
    {},
    {},
    {},
    {l5s0},
    {l6s0},
	{},
	{l8s0},
	{},
	{l10s0}
};


#define SHITTY_HARDCODED_LEVEL_INDEX 9

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
