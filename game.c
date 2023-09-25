// Libraries
#include <raylib.h>
#include <stdlib.h>
#include <math.h>

// Hint: To run every line, just press F6 (on Notepad++) or F5 (on VS Code) (if you're using another solution, idk)
int gameTime = 0;
const int FPS = 60;

const int SCREEN_WIDTH = 350;
const int SCREEN_HEIGHT = 450;
const int SCREEN_PADDING_TB = 10;
const int SCREEN_PADDING_LF = 14;

const double ENEMY_BLUE_HEIGHT = 28;
const double ENEMY_BLUE_WIDTH = 15;
const double ENEMY_BLUE_VELOCITY = 35;
const double ENEMY_BLUE_TTS = 2.5;
const double ENEMY_BLUE_TTM = 4.25;

typedef struct {
  Vector2 pos;
  int height;
  int width;
  double velocity;
  double TTS;
  double TTM;
  Color color;
} Enemy;

typedef Enemy* enemy;

enemy enemyActionSpawn(int height, int width, double velocity, double TTS, double TTM) {
  enemy e = malloc(sizeof(Enemy));

  int posX = GetRandomValue( SCREEN_PADDING_LF, SCREEN_WIDTH - width - SCREEN_PADDING_LF );
  Vector2 pos = { posX, SCREEN_PADDING_TB };

  e->pos = pos;
  e->height = height;
  e->width = width;
  e->velocity = velocity;
  e->TTS = TTS;
  e->TTM = TTM;
  e->color = BLUE;

  return e;
}

void enemyActionFall(enemy e) {
  if (e->pos.y > SCREEN_HEIGHT - SCREEN_PADDING_TB)
    e->pos.y = SCREEN_PADDING_TB;
  else e->pos.y += e->velocity;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------
    const int playerWidth = 60;
    const int playerHeight = 8;
    const Color playerColor = RED;
    const int playerVelocity = 4;

    Vector2 playerPos = {
      SCREEN_WIDTH / 2 - playerWidth / 2,
      SCREEN_HEIGHT - playerHeight - SCREEN_PADDING_TB
    };

    enemy e = enemyActionSpawn(
      ENEMY_BLUE_HEIGHT, ENEMY_BLUE_WIDTH,
      ENEMY_BLUE_VELOCITY, ENEMY_BLUE_TTS,
      ENEMY_BLUE_TTM
    );
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "--- [Raylib - First Attempt] ---");
    SetTargetFPS(FPS);    // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------
    // Main game loop
    while(!WindowShouldClose()) {    // Detect window close button or ESC key
        //----------------------------------------------------------------------------
        // Update
        gameTime++;

        // Enemies spawn
        /*int currentSpawnTime = time / enemyTTS;
        if ( gameTime < currentSpawnTime ) {
          enemyPos.x = GetRandomValue(
            screenPaddingLF,
            screenWidth - enemyWidth - screenPaddingLF
          );

          gameTime = currentSpawnTime;
        }*/

        // Enemies fall
        if ( !fmod(gameTime, e->TTM) ) enemyActionFall(e);

        if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && playerPos.x > SCREEN_PADDING_LF )
            playerPos.x -= playerVelocity;

        if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && playerPos.x < SCREEN_WIDTH - playerWidth - SCREEN_PADDING_LF )
            playerPos.x += playerVelocity;   
        //----------------------------------------------------------------------------
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(
              playerPos.x, playerPos.y,
              playerWidth, playerHeight,
              playerColor
            );    // Player
            DrawRectangle(
              e->pos.x, e->pos.y,
              e->width, e->height,
              e->color
            );    // Enemy
        EndDrawing();
        //----------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------
    CloseWindow();    // Close window and OpenGL context
    //--------------------------------------------------------------------------------
    
    return 0;
}