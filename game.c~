/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "gframework.h"
#include "gutil.h"
#include "raylib.h"

#include "levelEditor.h"
#include "gamestate.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    
    
    
    initFramework();
    loadAdditionalImage("./resources/game_over_skull.png");

    GameState* game = getGameState();

    // TODO
    // 07. better ui
    // 21. add more sounds for enemies
    // 22. death scream
    // 28. try rebuilding raylib with -sdl https://github.com/gen2brain/raylib-go/issues/354
    // 33. fix cpt unfortunate facial hair lol
    // 39. most blue monster variants have wrong stats
    // 48. make liquids affect gameplay
    // 59. player collisions are still fucked when near ledges
    // 63. title screen
    // 64. "logo"
    // 65. acredituj ballera https://opengameart.org/content/nes-shooter-music-5-tracks-3-jingles
    // 68. gameover jingle
    // 69. win jingle
    // 71. music doesnt loop correctly
    // 72. gameover art
    // 73.





    //gfullscreen();

    // Main game loop
    while (isGameRunning())
    {
        updateGameState(game);
        textF("fps %d", 10, 32, GetFPS());
        fUpdate();

        if (IsKeyPressed(KEY_F11)){
            gfullscreen();
        }

        if (IsKeyPressed(KEY_F10)){
            toggleMusic();
        }
    }

    
    disposeGameState(game);
	disposeFramework();

    return 0;
}
