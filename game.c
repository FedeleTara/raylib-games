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
  double TTS;  // Time To Spawn
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
const int SCREEN_PADDING_TB = 10;
const int SCREEN_PADDING_LF = 14;

const int PLAYER_HEIGHT = 8;
const int PLAYER_WIDTH = 60;

const int MAX_ENEMIES_ON_SCREEN = 3;
const int ENEMY_BLUE_HEIGHT = 28;
const int ENEMY_BLUE_WIDTH = 15;
const double ENEMY_BLUE_VELOCITY = 35;
const double ENEMY_BLUE_TTS = 2.5;
const double ENEMY_BLUE_TTM = 4.25;

player_t player;
enemy_t enemy;

//------------------------------------------------------------------------------------
// Functions Declaration
//------------------------------------------------------------------------------------
player_t playerActionSpawn(int height, int width);
void playerActionMove(player_t p);

enemy_t enemyActionSpawn(int height, int width, double velocity, double TTS, double TTM);
void enemyActionFall(enemy_t e);

void Init();
void GameLoop();
void Update();
void Draw();

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
    
    return 0;
}

//------------------------------------------------------------------------------------
// Functions Definition
//------------------------------------------------------------------------------------

// Player Functions ------------------------------------------------------------------
player_t playerActionSpawn(int height, int width) {
  player_t p = malloc(sizeof(Player));

  p->pos = (Vector2) {
    SCREEN_WIDTH / 2 - width / 2,
    SCREEN_HEIGHT - height - SCREEN_PADDING_TB
  };
  p->height = height;
  p->width = width;
  p->velocity = 4;
  p->color = RED;

  return p;
}

void playerActionMove(player_t p) {
  if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && p->pos.x > SCREEN_PADDING_LF )
    p->pos.x -= p->velocity;

  if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && p->pos.x < SCREEN_WIDTH - PLAYER_WIDTH - SCREEN_PADDING_LF )
    p->pos.x += p->velocity;
}

// Enemy Functions -------------------------------------------------------------------
enemy_t enemyActionSpawn(int height, int width, double velocity, double TTS, double TTM) {
  enemy_t e = malloc(sizeof(Enemy));

  int posX = GetRandomValue( SCREEN_PADDING_LF, SCREEN_WIDTH - width - SCREEN_PADDING_LF );

  e->pos = (Vector2) { posX, SCREEN_PADDING_TB };
  e->height = height;
  e->width = width;
  e->velocity = velocity;
  e->TTS = TTS;
  e->TTM = TTM;
  e->color = BLUE;

  return e;
}

void enemyActionFall(enemy_t e) {
  if (e->pos.y > SCREEN_HEIGHT - SCREEN_PADDING_TB)
    e->pos.y = SCREEN_PADDING_TB;
  else e->pos.y += e->velocity;
}

// Main Game Functions ---------------------------------------------------------------
void Init() {  // Initialization
  player = playerActionSpawn( PLAYER_HEIGHT, PLAYER_WIDTH );
  enemy = enemyActionSpawn(
    ENEMY_BLUE_HEIGHT, ENEMY_BLUE_WIDTH,
    ENEMY_BLUE_VELOCITY, ENEMY_BLUE_TTS,
    ENEMY_BLUE_TTM
  );

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "--- [Raylib - NAME_TO_DEFINE] ---");
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
  if ( !fmod(gameTime, enemy->TTM) ) enemyActionFall(enemy);
}

void Draw() {
  BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle(
      player->pos.x, player->pos.y,
      player->width, player->height,
      player->color
    );    // Player
    DrawRectangle(
      enemy->pos.x, enemy->pos.y,
      enemy->width, enemy->height,
      enemy->color
    );    // Enemy
  EndDrawing();
}