/*******************************************************************************************
*
*   raylib - standard game: FRUITS CATCHER (temporary)
*
*   Sample game developed by Fedele Tara (@FedeleTara)
*
*   This game has been created using raylib v4.5 (www.raylib.com)
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
  Vector2 size;
  Vector3 padding;
  Color background;

  int textSize;
  Color textColor;

  Vector2 infoBoxPos;
  Vector2 infoBoxSize;
  Color infoBoxColor;

  Vector2 infoTimePos;
  Vector2 infoWavePos;
  Vector2 infoScorePos;
} ScreenInfo;

typedef ScreenInfo* screenInfo_t;

typedef struct {
  double velocity;

  Vector2 pos;
  Vector2 size;

  Color color;
} Player;

typedef Player* player_t;

typedef struct {
  double velocity;
  double TTM;  // Time To Move

  Vector2 pos;
  Vector2 size;

  Color color;
} Enemy;

typedef Enemy* enemy_t;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
const char GAME_TITLE[] = "--- [FRUITS CATCHER - Made with <3 and Raylib] ---";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GAME_FPS = 60;

const int PLAYER_HEIGHT = 8;
const int PLAYER_WIDTH = 60;

const int ENEMY_BLUE_HEIGHT = 28;
const int ENEMY_BLUE_WIDTH = 15;
const double ENEMY_BLUE_VELOCITY = 3.75;
const double ENEMY_BLUE_TTM = 0.5;

int gameTime,
  enemySpawned,
  time,
  wave,
  score = 0;

screenInfo_t screen;
player_t player;
enemy_t enemies[MAX_ENEMIES_ON_SCREEN];

//------------------------------------------------------------------------------------
// Functions Declaration
//------------------------------------------------------------------------------------
void ScreenActionInit(int height, int width);

void PlayerActionSpawn(int height, int width);
void PlayerActionMove();

void EnemyActionSpawn(int index, int height, int width, double velocity, double TTM);
void EnemyActionFall(int index);

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

// Screen Functions ------------------------------------------------------------------
void ScreenActionInit(int height, int width) {
  screenInfo_t s = malloc(sizeof(ScreenInfo));
  
  int infoBoxHeight = height / 10;
  int innerInfoWidth = width / 3;
  int marginL = (innerInfoWidth / 100) * 3;
  int marginT = infoBoxHeight / 4;

  s->size = (Vector2) { width, height };
  s->padding = (Vector3) { infoBoxHeight, width / 100, (height / 100) * 3 };  // top, left/right, bottom
  s->background = RAYWHITE;

  s->textSize = infoBoxHeight / 2;
  s->textColor = BLACK;

  s->infoBoxPos = (Vector2) { 0, 0 };
  s->infoBoxSize = (Vector2) { width, infoBoxHeight};
  s->infoTimePos = (Vector2) { marginL, marginT };
  s->infoWavePos = (Vector2) { innerInfoWidth + marginL, marginT };
  s->infoScorePos = (Vector2) { innerInfoWidth * 2 + marginL, marginT };
  s->infoBoxColor = ORANGE;

  screen = s;
}

// Player Functions ------------------------------------------------------------------
void PlayerActionSpawn(int height, int width) {
  player_t p = malloc(sizeof(Player));

  p->pos = (Vector2) {
    screen->size.x / 2 - width / 2,
    screen->size.y - height - screen->padding.z
  };
  p->size = (Vector2) { width, height };
  p->velocity = 4;
  p->color = RED;

  player = p;
}

void PlayerActionMove() {
  if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && player->pos.x > screen->padding.y )
    player->pos.x -= player->velocity;

  if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && player->pos.x < screen->size.x - player->size.x - screen->padding.y )
    player->pos.x += player->velocity;
}

// Enemy Functions -------------------------------------------------------------------
void EnemyActionSpawn(int index, int height, int width, double velocity, double TTM) {
  enemy_t e = malloc(sizeof(Enemy));

  int posX = GetRandomValue( screen->padding.y, screen->size.x - width - screen->padding.y );

  e->pos = (Vector2) { posX, screen->padding.x };
  e->size = (Vector2) { width, height };
  e->velocity = velocity;
  e->TTM = TTM;
  e->color = BLUE;

  enemies[index] = e;
}

void EnemyActionFall(int index) {
  enemy_t e = enemies[index];

  if (e->pos.y < screen->size.y - screen->padding.z - e->size.y)
    e->pos.y += e->velocity;
  else e->pos.y = screen->padding.x;
}

// Main Game Functions ---------------------------------------------------------------
void Init() {  // Initialization
  ScreenActionInit( SCREEN_HEIGHT, SCREEN_WIDTH );
  PlayerActionSpawn( PLAYER_HEIGHT, PLAYER_WIDTH );
  InitWindow( SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE );
  SetTargetFPS( GAME_FPS );  // Set our game to run at 60 frames-per-second
}

void GameLoop() {  // Main game loop
  while(!WindowShouldClose()) {  // Detect window close button or ESC key
    Update();
    Draw();
  }
}

void Update() {
  time = GetTime();
  gameTime++;

  PlayerActionMove(player);  // Player moves

  for (int i = 0; i < enemySpawned; i++) { // Enemies action
    enemy_t e = enemies[i];
    
    // Other actions...
    // TODO

    // Enemies fall
    if ( !fmod(gameTime, e->TTM) ) EnemyActionFall(i);
  }

  if (  // Enemies spawn (for now, only blue rectangles :D)
      enemySpawned < MAX_ENEMIES_ON_SCREEN && !fmod(gameTime, ENEMY_BLUE_TTS)
    ) {
      EnemyActionSpawn(
        enemySpawned,
        ENEMY_BLUE_HEIGHT, ENEMY_BLUE_WIDTH,
        ENEMY_BLUE_VELOCITY, ENEMY_BLUE_TTM
      );
      enemySpawned++;
  }
}

void Draw() {
  BeginDrawing();

    ClearBackground( screen->background );

    // Info -----------------------------------------------------------------
    DrawRectangleV( screen->infoBoxPos, screen->infoBoxSize, screen->infoBoxColor );
    DrawText(
      TextFormat("time %i", time),
      screen->infoTimePos.x, screen->infoTimePos.y,
      screen->textSize, screen->textColor
    );
    DrawText(
      TextFormat("wave %i", wave),
      screen->infoWavePos.x, screen->infoWavePos.y,
      screen->textSize, screen->textColor
    );
    DrawText(
      TextFormat("score %i", score),
      screen->infoScorePos.x, screen->infoScorePos.y,
      screen->textSize, screen->textColor
    );

    // Player ---------------------------------------------------------------
    DrawRectangleV( player->pos, player->size, player->color );

    // Enemies --------------------------------------------------------------
    for (int i = 0; i < enemySpawned; i++) {
      enemy_t e = enemies[i];
      DrawRectangleV( e->pos, e->size, e->color );
    }

  EndDrawing();
}

void FreeMemory() {
  free(player);

  for (int i = 0; i < enemySpawned; i++) {
    enemy_t e = enemies[i];
    free(e);
  }

  free(screen);
} 