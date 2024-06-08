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
#include "gframework.c"
#include "raylib.h"
#include "level.c"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    initFramework();
    
    File* f = initFile("./gamedata/1.lvl");
    char data[] = {0, 2, 0, 2, 1, 0, 0, 2};
    f->contents = data;
    f->contentsLength = 8;
    saveFile(f);
    printf("file saved\n");
    
    Level* l = loadLevel("./gamedata/1.lvl");
    unloadLevel(l);



    // Main game loop
    while (!WindowShouldClose())
    {
        
        fDrawBegin();

            ClearBackground(BLACK);

            
            
            

        fDrawEnd();
        
    }

	disposeFramework();
    

    return 0;
}
