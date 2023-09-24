// Libraries
#include <raylib.h>

// Hint: To run every line, just press F6 (on Notepad++, if you're using another solution, idk)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------
    // TODO Declare some stuff
    int screenWidth = 500;
    int screenHeight = 300;
    
    InitWindow(screenWidth, screenHeight, "--- [Raylib - Blank (Yellow) Init] ---");
    SetTargetFPS(60);    // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------
    // Main game loop
    while(!WindowShouldClose()) {    // Detect window close button or ESC key
        // Update
        // TODO Update something and go on
        //----------------------------------------------------------------------------
        // Draw
        BeginDrawing();
        // TODO Draw your mess and repeat
        ClearBackground(YELLOW);
        EndDrawing();
        //----------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------
    CloseWindow();    // Close window and OpenGL context
    //--------------------------------------------------------------------------------
    
    return 0;
}