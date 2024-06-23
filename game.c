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
#include "raylib.h"
#include "levelEditor.h"
#include "gameplay.h"
#include "entities.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    initFramework();
    
    // TODO main menu of some kind

    //LevelEditor* l = initLevelEditor("./gamedata/1.lvl");
    Gameplay* g = startGameplay("./gamedata/1.lvl");


    // Main game loop
    while (!WindowShouldClose())
    {
            
        //updateLevelEditor(l);
        updateGameplay(g);
        
        fUpdate();
    }
    //unloadLevelEditor(l);
    stopGameplay(g);
	disposeFramework();
    //unloadEntityManager(getEntityManager());

    return 0;
}
