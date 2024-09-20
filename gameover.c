#include "gameover.h"
#include "gframework.h"
#include "spritedata.h"
#include "gamestate.h"


#define TEXT_X SCREEN_WIDTH / 2 - 60
#define TEXT_Y SCREEN_HEIGHT / 4

#define BUTTON1_X SCREEN_WIDTH / 2 - 160
#define BUTTON2_X SCREEN_WIDTH / 2 + 100
#define BUTTON_Y SCREEN_HEIGHT / 2 + 140

#define SKULL_X SCREEN_WIDTH / 2 - 110
#define SKULL_Y SCREEN_HEIGHT / 2 - 130

char selectedOption = 0;
void updateGameOverScreen(){
    // main text
    textF("game over", TEXT_X, TEXT_Y);

    // buttons
    textF("exit", BUTTON1_X, BUTTON_Y);
    textF("continue", BUTTON2_X, BUTTON_Y);

    // skull
    drawAdditionalImage(0, SKULL_X, SKULL_Y, DEFAULT_CAMERA_ZOOM, LAYER_STATIC_UI);


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
            continueLevel(getGameState());
        }else {
            activateScreenTransition(getGameState(), GAME_STATE_MAIN_MENU);
        }
    }

}


void resetGameOverScreen(){
    selectedOption = 0;
}
