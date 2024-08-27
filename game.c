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
    // 1. get some decent tunes
    // 2. sometimes camera flashes when changing markers
    // 3. scripting
    // 7. better ui
    // 8. actually functional main menu
    // 10. clean up console
    // 13. player to world collisions are still fucked
    // 14. enemies dont despawn properly?
    // 20. goomba stomping ????
    // 21. add more sounds for enemies
    // 22. death scream
    // 23. projectile shoot sounds
    // 24. level exit
    // 25. flame particles
    // 27. fix fullscreen // shits fucked on wayland
    // 28. try rebuilding raylib with -sdl https://github.com/gen2brain/raylib-go/issues/354
    // 30. lvl 1
    // 31.





    //gfullscreen();

    // Main game loop
    while (!WindowShouldClose())
    {
        updateGameState(game);
        textF("fps %d", 10, 10, GetFPS());
        fUpdate();

        if (IsKeyPressed(KEY_F11)){
            gfullscreen();
        }
    }

    
    disposeGameState(game);
	disposeFramework();
    

    return 0;
}
