/*******************************************************************************************
*
*   raylib - standard game: NAME_TO_DEFINE
*
*   Sample game developed by Fedele Tara (@FedeleTara)
*
*   This game has been created using raylib v4.x (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2023 Fedele Tara (@FedeleTara) and Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ENEMIES_ON_SCREEN 3
#define ENEMY_BLUE_TTS 100 // Time To Spawn

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct {
  Vector2 pos;
  int height;
  int width;
  double velocity;
  Color color;
} Player;

typedef Player* player_t;

typedef struct {
  Vector2 pos;
  int height;
  int width;
  double velocity;
  double TTM;  // Time To Move
  Color color;
} Enemy;

typedef Enemy* enemy_t;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
int gameTime = 0;
const int FPS = 60;

const int SCREEN_HEIGHT = 450;
const int SCREEN_WIDTH = 350;
const char GAME_TITLE[] = "--- [Raylib - NAME_TO_DEFINE] ---";
const int SCREEN_PADDING_TB = 10;
const int SCREEN_PADDING_LF = 14;

const int PLAYER_HEIGHT = 8;
const int PLAYER_WIDTH = 60;

const int ENEMY_BLUE_HEIGHT = 28;
const int ENEMY_BLUE_WIDTH = 15;
const double ENEMY_BLUE_VELOCITY = 35;
const double ENEMY_BLUE_TTM = 4.25;

player_t player;
enemy_t enemies[MAX_ENEMIES_ON_SCREEN];

//------------------------------------------------------------------------------------
// Functions Declaration
//------------------------------------------------------------------------------------
void playerActionSpawn(int height, int width);
void playerActionMove();

void enemyActionSpawn(int index, int height, int width, double velocity, double TTM);
void enemyActionFall(int index);

void Init();
void GameLoop();
void Update();
void Draw();
void FreeMemory();

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    Init();

    // Main game loop
    GameLoop();

    // De-Initialization
    CloseWindow();    // Close window and OpenGL context
    
    // Free all the previous allocated memory
    FreeMemory();

    return 0;
}

//------------------------------------------------------------------------------------
// Functions Definition
//------------------------------------------------------------------------------------

// Player Functions ------------------------------------------------------------------
void playerActionSpawn(int height, int width) {
  player_t p = malloc(sizeof(Player));

  p->pos = (Vector2) {
    SCREEN_WIDTH / 2 - width / 2,
    SCREEN_HEIGHT - height - SCREEN_PADDING_TB
  };
  p->height = height;
  p->width = width;
  p->velocity = 4;
  p->color = RED;

  player = p;
}

void playerActionMove() {
  if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && player->pos.x > SCREEN_PADDING_LF )
    player->pos.x -= player->velocity;

  if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && player->pos.x < SCREEN_WIDTH - PLAYER_WIDTH - SCREEN_PADDING_LF )
    player->pos.x += player->velocity;
}

// Enemy Functions -------------------------------------------------------------------
void enemyActionSpawn(int index, int height, int width, double velocity, double TTM) {
  enemy_t e = malloc(sizeof(Enemy));

  int posX = GetRandomValue( SCREEN_PADDING_LF, SCREEN_WIDTH - width - SCREEN_PADDING_LF );

  e->pos = (Vector2) { posX, SCREEN_PADDING_TB };
  e->height = height;
  e->width = width;
  e->velocity = velocity;
  e->TTM = TTM;
  e->color = BLUE;

  enemies[index] = e;
}

void enemyActionFall(int index) {
  enemy_t e = enemies[index];

  if (e->pos.y > SCREEN_HEIGHT - SCREEN_PADDING_TB)
    e->pos.y = SCREEN_PADDING_TB;
  else e->pos.y += e->velocity;
}

// Main Game Functions ---------------------------------------------------------------
void Init() {  // Initialization
  playerActionSpawn( PLAYER_HEIGHT, PLAYER_WIDTH );

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
  SetTargetFPS(FPS);    // Set our game to run at 60 frames-per-second
}

void GameLoop() {  // Main game loop
  while(!WindowShouldClose()) {    // Detect window close button or ESC key
    Update();
    Draw();
  }
}

void Update() {
  gameTime++;

  // Player moves
  playerActionMove(player);

  bool spawn = true;
  for (int i = 0; i < MAX_ENEMIES_ON_SCREEN; i++) {
    enemy_t e = enemies[i];
    if (e != NULL) {  // Enemies actions
      // Other actions...
      // TODO

      // Enemies fall
      if ( !fmod(gameTime, e->TTM) ) enemyActionFall(i);
    } else if (  // Enemies spawn
      !fmod(gameTime, ENEMY_BLUE_TTS) && spawn
    ) {
      enemyActionSpawn(
        i,
        ENEMY_BLUE_HEIGHT, ENEMY_BLUE_WIDTH,
        ENEMY_BLUE_VELOCITY, ENEMY_BLUE_TTM
      );
      spawn = false;
    }
  }
}

void Draw() {
  BeginDrawing();
    ClearBackground(RAYWHITE);
    // Player ---------------------------------------------------------------
    DrawRectangle(
      player->pos.x, player->pos.y,
      player->width, player->height,
      player->color
    );
    // Enemies --------------------------------------------------------------
    for (int i = 0; i < MAX_ENEMIES_ON_SCREEN; i++) {
      enemy_t e = enemies[i];
      if (e != NULL)
        DrawRectangle(
          e->pos.x, e->pos.y,
          e->width, e->height,
          e->color
        );
    }
  EndDrawing();
}

void FreeMemory() {
  free(player);
  for (int i = 0; i < MAX_ENEMIES_ON_SCREEN; i++) {
    enemy_t e = enemies[i];
    if (e != NULL) free(e);
  }
} 