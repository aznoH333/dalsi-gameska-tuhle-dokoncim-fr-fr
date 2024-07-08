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


    GameState* game = getGameState();

    // TODO
    // 1. super rare bug that skips current camera point (cause unknown)
    // 2. get some decent tunes
    // 3. enemy marker interaction

    // Main game loop
    while (!WindowShouldClose())
    {
        updateGameState(game);
        textF("fps %d", 10, 10, GetFPS());
        fUpdate();
    }
    
    //disposeGameState(game);
	disposeFramework();
    

    return 0;
}
