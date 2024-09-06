#include "gameover.h"
#include "gframework.h"
#include "spritedata.h"
#include "gamestate.h"


#define TEXT_X SCREEN_WIDTH / 2 - 60
#define TEXT_Y SCREEN_HEIGHT / 2

#define BUTTON1_X SCREEN_WIDTH / 2 - 160
#define BUTTON2_X SCREEN_WIDTH / 2 + 100
#define BUTTON_Y SCREEN_HEIGHT / 2 + 60

char selectedOption = 0;
void updateGameOverScreen(){
    // main text
    textF("game over", TEXT_X, TEXT_Y);

    // buttons
    textF("exit", BUTTON1_X, BUTTON_Y);
    textF("continue", BUTTON2_X, BUTTON_Y);


    // selector
    int x = -32;

    if (selectedOption == 1){
        x += BUTTON1_X;
    }else {
        x += BUTTON2_X;
    }

    drawS(CURSOR_SPRITE, x, BUTTON_Y - 8, 2.0f, LAYER_STATIC_UI);

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)){
        selectedOption++;
        selectedOption %= 2;
    }   

    // activation
    if (IsKeyPressed(KEY_ENTER)){
        if (selectedOption == 0){
            gLog(LOG_ERR, "not implemented");
        }else {
            activateScreenTransition(getGameState(), GAME_STATE_MAIN_MENU);
        }
    }

}


void resetGameOverScreen(){
    selectedOption = 0;
}
