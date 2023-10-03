/*******************************************************************************************
*
*   raylib - classic game: FRUITS CATCHER (temporary)
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

//----------------------------------------------------------------------------------
// Macro Definition
//----------------------------------------------------------------------------------

#define MAX_ENEMIES_ON_SCREEN 100
#define ENEMY_BLUE_TTS 50  // Time To Spawn

//----------------------------------------------------------------------------------
// Enums Definition
//----------------------------------------------------------------------------------

typedef enum {
  BASE = 1,
  ADVANCED = 2,
  BOSS = 3
} EnemyType;

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct {
  Vector2 size;
  Vector3 padding;
  Color background;

  int textSize;
  Color textColor;

  Rectangle infoBoxRect;
  Color infoBoxColor;

  Vector2 infoTimePos;
  Vector2 infoWavePos;
  Vector2 infoScorePos;
} ScreenInfo;

typedef struct {
  double velocity;

  Rectangle rect;
  Color color;
} Player;

typedef struct {
  double ttm;
  double velocity;
  bool spawned;

  EnemyType type;
  Rectangle rect;
  Color color;
} Enemy;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
const static char GAME_TITLE[] = "--- [FRUITS CATCHER - Made with <3 and Raylib] ---";
const static int SCREEN_WIDTH = 640;
const static int SCREEN_HEIGHT = 480;
const static int GAME_FPS = 60;

const static int PLAYER_HEIGHT = 8;
const static int PLAYER_WIDTH = 60;

const static int ENEMY_BLUE_HEIGHT = 28;
const static int ENEMY_BLUE_WIDTH = 15;
const static double ENEMY_BLUE_TTM = 0.5;  // Time to Mode
const static double ENEMY_BLUE_VELOCITY = 3.75;

static int gameTime = 0;
static int enemySpawned = 0;
static int time = 0;
static int wave = 0;
static int score = 0;

static ScreenInfo screen = { 0 };
static Player player = { 0 };
static Enemy enemies[MAX_ENEMIES_ON_SCREEN] = { 0 };

//------------------------------------------------------------------------------------
// Functions Declaration
//------------------------------------------------------------------------------------
static void ScreenActionInit(void);

static void PlayerActionSpawn(void);
static void PlayerActionMove(void);

static void EnemyActionSpawn(EnemyType type);
static void EnemyActionFall(int index);
static void EnemyActionCollect(int index);

static void Init(void);
static void GameLoop(void);
static void Update(void);
static void Draw(void);
static void Unload(void);

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
    Unload();

    return 0;
}

//------------------------------------------------------------------------------------
// Functions Definition
//------------------------------------------------------------------------------------

// Screen Functions ------------------------------------------------------------------
void ScreenActionInit(void) {
  int infoBoxHeight = SCREEN_HEIGHT / 10;
  int innerInfoWidth = SCREEN_WIDTH / 3;
  int marginL = (innerInfoWidth / 100) * 3;
  int marginT = infoBoxHeight / 4;

  screen.size = (Vector2) { SCREEN_WIDTH, SCREEN_HEIGHT };
  screen.padding = (Vector3) { infoBoxHeight, SCREEN_WIDTH / 100, (SCREEN_HEIGHT / 100) * 3 };  // top, left/right, bottom
  screen.background = RAYWHITE;

  screen.textSize = infoBoxHeight / 2;
  screen.textColor = BLACK;

  screen.infoBoxRect = (Rectangle) { 0, 0, SCREEN_WIDTH, infoBoxHeight };
  screen.infoTimePos = (Vector2) { marginL, marginT };
  screen.infoWavePos = (Vector2) { innerInfoWidth + marginL, marginT };
  screen.infoScorePos = (Vector2) { innerInfoWidth * 2 + marginL, marginT };
  screen.infoBoxColor = ORANGE;
}

// Player Functions ------------------------------------------------------------------
void PlayerActionSpawn(void) {
  player.rect = (Rectangle) {
    screen.size.x / 2 - PLAYER_WIDTH / 2,
    screen.size.y - PLAYER_HEIGHT - screen.padding.z,
    PLAYER_WIDTH, PLAYER_HEIGHT
  };
  player.velocity = 4;
  player.color = RED;
}

void PlayerActionMove(void) {
  if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && player.rect.x > screen.padding.y )
    player.rect.x -= player.velocity;

  if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && player.rect.x < screen.size.x - player.rect.width - screen.padding.y )
    player.rect.x += player.velocity;
}

// Enemy Functions -------------------------------------------------------------------
void EnemyActionSpawn(EnemyType type) {
  for (int i = 0; i < MAX_ENEMIES_ON_SCREEN; i++) {
    if (!enemies[i].spawned) {
      if (type == BASE) {
        int posX = GetRandomValue( screen.padding.y, screen.size.x - ENEMY_BLUE_WIDTH - screen.padding.y );

        enemies[i].rect = (Rectangle) {
          posX, screen.padding.x,
          ENEMY_BLUE_WIDTH, ENEMY_BLUE_HEIGHT
        };
        enemies[i].ttm = ENEMY_BLUE_TTM;
        enemies[i].velocity = ENEMY_BLUE_VELOCITY;
        enemies[i].color = BLUE;

        enemies[i].spawned = true;
      }

      break;
    }
  }
}

void EnemyActionFall(int index) {
  if (
    enemies[index].rect.y <
    screen.size.y - screen.padding.z - enemies[index].rect.height
  )
    enemies[index].rect.y += enemies[index].velocity;
  else {
    // TODO Play animation "Rot"
    enemies[index] = (Enemy) { 0 };
    enemySpawned--;
  }
}

void EnemyActionCollect(int index) {
  // TODO Play animation "Collect"
  enemies[index] = (Enemy) { 0 };
  enemySpawned--;
  score++;
}

// Main Game Functions ---------------------------------------------------------------
void Init(void) {  // Initialization
  ScreenActionInit();
  PlayerActionSpawn();
  InitWindow( SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE );
  SetTargetFPS( GAME_FPS );  // Set our game to run at 60 frames-per-second
}

void GameLoop(void) {  // Main game loop
  while(!WindowShouldClose()) {  // Detect window close button or ESC key
    Update();
    Draw();
  }
}

void Update(void) {
  time = GetTime();
  gameTime++;

  // Player ---------------------------------------------------------------
  PlayerActionMove();

  // Enemies --------------------------------------------------------------
  for (
    int i = 0, j = 0;
    j < enemySpawned && i < MAX_ENEMIES_ON_SCREEN;
    i++
  ) {
    if (enemies[i].spawned) {
      // Enemies fall
      if ( !fmod(gameTime, enemies[i].ttm) )
        EnemyActionFall(i);
      
      // Other actions...
      // TODO

      // Detect Collisions
      if( CheckCollisionRecs(player.rect, enemies[i].rect) )
        EnemyActionCollect(i);

      // Increment the counter
      j++;
    }
  }

  if (  // Enemies spawn (for now, only blue rectangles :D)
      enemySpawned < MAX_ENEMIES_ON_SCREEN && !fmod(gameTime, ENEMY_BLUE_TTS)
    ) {
      EnemyActionSpawn( BASE );
      enemySpawned++;
  }
}

void Draw(void) {
  BeginDrawing();

    ClearBackground( screen.background );

    // Info -----------------------------------------------------------------
    DrawRectangleRec( screen.infoBoxRect, screen.infoBoxColor );
    DrawText(
      TextFormat("time %i", time),
      screen.infoTimePos.x, screen.infoTimePos.y,
      screen.textSize, screen.textColor
    );
    DrawText(
      TextFormat("wave %i", wave),
      screen.infoWavePos.x, screen.infoWavePos.y,
      screen.textSize, screen.textColor
    );
    DrawText(
      TextFormat("score %i", score),
      screen.infoScorePos.x, screen.infoScorePos.y,
      screen.textSize, screen.textColor
    );

    // Player ---------------------------------------------------------------
    DrawRectangleRec( player.rect, player.color );

    // Enemies --------------------------------------------------------------
    for (
      int i = 0, j = 0;
      j < enemySpawned && i < MAX_ENEMIES_ON_SCREEN;
      i++
    ) {
      if (enemies[i].spawned) {
        DrawRectangleRec( enemies[i].rect, enemies[i].color );
        j++;
      }
    }

  EndDrawing();
}

void Unload(void) {  // Unload game variables
  // :Unload here all dynamic loaded data (textures, sounds, models...)
}