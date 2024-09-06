#include "mainMenu.h"
#include "gframework.h"
#include "spritedata.h"
#include "gamestate.h"

int selectedOtion = 0;
#define MAX_OPTIONS 4

const char* OPTION_TEXTS[MAX_OPTIONS] = {
    "new game",
    "continue",
    "fullscreen",
        "exit",
};
// text is not affected by camera zoom
#define OPTION_START_X SCREEN_WIDTH / 2
#define OPTION_START_Y SCREEN_HEIGHT / 2
#define OPTION_GAP 32
#define CURSOR_GAP 32

void handleButtonFunctions(int buttonIndex){
    switch (buttonIndex) {
        case 0: // new game
            setNextLevelIndex(getGameState(), 0);
            startCurrentLevel(getGameState());
            break;
        case 1: // continue
            continueLevel(getGameState());
            break;
        case 2: // fullscreen
            gfullscreen();
            break;
        case 3: // exit
            closeGame();
            break;
    }
}


void updateMainMenu(){
    // draw
    for (int i = 0; i < MAX_OPTIONS; i++){
        int offset = strLength(OPTION_TEXTS[i]) * 8;
        
        textF("%s", OPTION_START_X - offset, (OPTION_START_Y + i * OPTION_GAP), OPTION_TEXTS[i]);

        // draw selector
        if (selectedOtion == i){
            drawFS(CURSOR_SPRITE, OPTION_START_X - offset - CURSOR_GAP, (OPTION_START_Y + i * OPTION_GAP) - 8, false, 2.0f, LAYER_STATIC_UI);
            drawFS(CURSOR_SPRITE, OPTION_START_X + (offset) + CURSOR_GAP - 32, (OPTION_START_Y + i * OPTION_GAP) - 8, true, 2.0f, LAYER_STATIC_UI);

        }
    }

    // update cursor
    if (IsKeyPressed(KEY_UP)){
        selectedOtion--;
        if (selectedOtion < 0){
            selectedOtion = MAX_OPTIONS - 1;
        }
    }else if (IsKeyPressed(KEY_DOWN)){
        selectedOtion++;
        if (selectedOtion >= MAX_OPTIONS){
            selectedOtion = 0;
        }
    }

    if (IsKeyPressed(KEY_ENTER)){
        handleButtonFunctions(selectedOtion);
    }

}

void resetMainMenu(){
    selectedOtion = 0;
}