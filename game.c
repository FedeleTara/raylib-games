// Libraries
#include <raylib.h>

// Hint: To run every line, just press F6 (on Notepad++) or F5 (on VS Code) (if you're using another solution, idk)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------
    int gameTime = 0;

    const int screenWidth = 350;
    const int screenHeight = 450;
    const int screenPaddingTB = 10;
    const int screenPaddingLF = 14;
    
    const int playerWidth = 60;
    const int playerHeight = 8;
    const int playerVelocity = 4;

    const int enemyTTS = 3;    // Time To Spawn (in this case, every 3 seconds)
    const int enemyWidth = 15;
    const int enemyHeight = 28;

    Vector2 playerPos = {
      screenWidth / 2 - playerWidth / 2,
      screenHeight - playerHeight - screenPaddingTB
    };

    Vector2 enemyPos = { screenPaddingLF, screenPaddingTB };
    
    InitWindow(screenWidth, screenHeight, "--- [Raylib - First Attempt] ---");
    SetTargetFPS(60);    // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------
    // Main game loop
    while(!WindowShouldClose()) {    // Detect window close button or ESC key
        //----------------------------------------------------------------------------
        // Update
        int currentTime = (GetTime() / enemyTTS);

        if ( gameTime < currentTime ) {
          enemyPos.x = GetRandomValue(
            screenPaddingLF,
            screenWidth - enemyWidth - screenPaddingLF
          );

          gameTime = currentTime;
        }

        if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && playerPos.x > screenPaddingLF )
            playerPos.x -= playerVelocity;

        if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && playerPos.x < screenWidth - playerWidth - screenPaddingLF )
            playerPos.x += playerVelocity;   
        //----------------------------------------------------------------------------
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(
              playerPos.x, playerPos.y,
              playerWidth, playerHeight,
              RED
            );
            DrawRectangle(
              enemyPos.x, enemyPos.y,
              enemyWidth, enemyHeight,
              BLUE
            );
        EndDrawing();
        //----------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------
    CloseWindow();    // Close window and OpenGL context
    //--------------------------------------------------------------------------------
    
    return 0;
}