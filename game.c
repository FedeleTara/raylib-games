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

#define RAYGUI_IMPLEMENTATION
#include <lib/raygui.h>
#undef RAYGUI_IMPLEMENTATION  // Avoid including raygui implementation again

#include <stdlib.h>
#include <math.h>

//----------------------------------------------------------------------------------
// Macro Definition
//----------------------------------------------------------------------------------
#define MAX_ENEMIES_ON_SCREEN 25
#define ENEMY_BLUE_TTS 5  // Time To Spawn

//----------------------------------------------------------------------------------
// Enums Definition
//----------------------------------------------------------------------------------
typedef enum {
  BLUE_APPLE = 1,
  GREEN_BANANA = 2,
  RED_LEMON = 3
} EnemyType;

typedef enum {
  OVER = 0,
  IN_GAME = 1,
  OVERTIME = 2,
  WIN = 3,
} GameState;

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
const static char LABEL_TITLE[] = "--- [FRUITS CATCHER - Made with <3 and Raylib] ---";
const static char LABEL_GAMEOVER[] = "GAME OVER";
const static char LABEL_WIN[] = "YOU WIN";
const static char LABEL_OVERTIME[] = "OVERTIME";
const static char LABEL_RETRY[] = "RETRY?";
const static char LABEL_CHOISE[] = "Y (Yes) / N (No)";

const static int SCREEN_WIDTH = 640;
const static int SCREEN_HEIGHT = 480;
const static int GAME_FPS = 60;
const static int MAX_TIME = 30;
const static int MAX_SCORE = 110;

const static int PLAYER_HEIGHT = 8;
const static int PLAYER_WIDTH = 60;

const static int ENEMY_BLUE_HEIGHT = 28;
const static int ENEMY_BLUE_WIDTH = 15;
const static double ENEMY_BLUE_TTM = 0.5;  // Time to Mode
const static double ENEMY_BLUE_VELOCITY = 3.75;

static int gameTime = 0;
static int spawnedEnemies = 0;
static int timeStart = 0;
static int timePassed = 0;
static int wave = 0;
static int score = 0;

static GameState gamestate = IN_GAME;
static ScreenInfo screen = { 0 };
static Player player = { 0 };
static Enemy enemies[MAX_ENEMIES_ON_SCREEN] = { 0 };

//------------------------------------------------------------------------------------
// Functions Declaration
//------------------------------------------------------------------------------------
static void PlayerActionSpawn(void);
static void PlayerActionMove(void);

static void EnemyActionSpawn(EnemyType type);
static bool EnemyActionFall(int index);
static void EnemyActionCollect(int index);

static void ScreenInit(void);
static void GameOver(GameState);
static void Restart(void);

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

// Player Functions ------------------------------------------------------------------
void PlayerActionSpawn(void) {
  // TODO Play animation "Spawn Player"
  player.rect = (Rectangle) {
    screen.size.x / 2 - PLAYER_WIDTH / 2,
    screen.size.y - PLAYER_HEIGHT - screen.padding.z,
    PLAYER_WIDTH, PLAYER_HEIGHT
  };
  player.velocity = 4;
  player.color = RED;
}

void PlayerActionMove(void) {
  if (
      (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      && player.rect.x > screen.padding.y
    ) {
    player.rect.x -= player.velocity;
  }

  if (
      (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      && player.rect.x < screen.size.x - player.rect.width - screen.padding.y
    ) {
    player.rect.x += player.velocity;
  }
}

// Enemy (main) Functions -------------------------------------------------------------------
void EnemyActionSpawn(EnemyType type) {
  for (int i = 0; i < MAX_ENEMIES_ON_SCREEN; i++) {
    if (!enemies[i].spawned) {
      if (type == BLUE_APPLE) {
        // TODO Play animation "Spawn Blue"
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

bool EnemyActionFall(int index) {
  if (
    enemies[index].rect.y <
    screen.size.y - screen.padding.z - enemies[index].rect.height
  ) {
    // TODO Play animation "Fall"
    enemies[index].rect.y += enemies[index].velocity;
  } else {
    // TODO Play animation "Rot"
    enemies[index] = (Enemy) { 0 };
    return true; // The fruit has rotten at this point
  }
  
  return false; // The fruit is falling yet
}

void EnemyActionCollect(int index) {
  // TODO Play animation "Collect"
  enemies[index] = (Enemy) { 0 };
  score++;
}

// Main Game (tools) Functions ---------------------------------------------------------------
void ScreenInit(void) {
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

void GameOver(GameState state) {
  // TODO Play animation "Game Over"
  player = (Player) { 0 };
  
  for (
    int i = 0, j = 0;
    j < spawnedEnemies && i < MAX_ENEMIES_ON_SCREEN;
    i++
  ) {
    if (enemies[i].spawned) {
      enemies[i] = (Enemy) { 0 };
    }
  }

  gamestate = state;
}

void Restart(void) {  // New Game
  // Reset all the variables for a new game
  gameTime = 0;
  spawnedEnemies = 0;
  timeStart = GetTime();
  wave = 0;
  score = 0;

  PlayerActionSpawn();

  gamestate = IN_GAME;
}

// Main Game (essentials) Functions ---------------------------------------------------------------
void Init(void) {  // Initialization
  ScreenInit();
  PlayerActionSpawn();
  InitWindow( SCREEN_WIDTH, SCREEN_HEIGHT, LABEL_TITLE );
  SetTargetFPS( GAME_FPS );  // Set our game to run at 60 frames-per-second
}

void GameLoop(void) {  // Main game loop
  while( !WindowShouldClose() && gamestate != OVER ) {  // Detect window close button or ESC key
    Update();
    Draw();
  }
}

void Update(void) {
  if ( gamestate == IN_GAME ) {
    // Declarations section
    int deSpawnedEnemies = 0;

    // Time (Local) section
    timePassed = floor(GetTime() - timeStart);
    if ( timePassed >= MAX_TIME ) {
      GameOver(OVERTIME);
      return;
    }
    
    // Score section
    if ( score >= MAX_SCORE) {
      GameOver(WIN);
      return;
    }

    // Everything else
    gameTime++;

    // Player ---------------------------------------------------------------
    PlayerActionMove();

    // Enemies --------------------------------------------------------------
    for (
      int i = 0, j = 0;
      j < spawnedEnemies && i < MAX_ENEMIES_ON_SCREEN;
      i++
    ) {
      if (enemies[i].spawned) {
        // Increment the counter
        j++;

        // Enemies fall
        if ( !fmod(gameTime, enemies[i].ttm) ) {
          if ( EnemyActionFall(i) ) {
            deSpawnedEnemies++;
            continue;
          }
        }

        // Other actions...
        // TODO

        // Detect Collisions
        if ( CheckCollisionRecs(player.rect, enemies[i].rect) ) {
          EnemyActionCollect(i);
          deSpawnedEnemies++;
          continue;
        }
      }
    }

    if (  // Enemies spawn (for now, only blue rectangles :D)
        spawnedEnemies < MAX_ENEMIES_ON_SCREEN
        && !fmod(gameTime, ENEMY_BLUE_TTS)
      ) {
        EnemyActionSpawn( BLUE_APPLE );
        spawnedEnemies++;
    }

    spawnedEnemies -= deSpawnedEnemies;
  } else {

    if (
      // End the game
      IsKeyDown(KEY_N)
      ) {
      gamestate = OVER;
    } else if (
      // Start a new game
      IsKeyDown(KEY_Y)
      ) {
      Restart();
    }
  
  }
}

void Draw(void) {
  // If something ends the game, it should stops drawing things
  if ( gamestate == OVER ) {
    return;
  }

  BeginDrawing();

    ClearBackground( screen.background );

    // Info -----------------------------------------------------------------
    DrawRectangleRec( screen.infoBoxRect, screen.infoBoxColor );
    DrawText(
      TextFormat("time %i", timePassed),
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

    switch ( gamestate ) {
      case IN_GAME:
        // Player ---------------------------------------------------------------
        DrawRectangleRec( player.rect, player.color );

        // Enemies --------------------------------------------------------------
        for (
          int i = 0, j = 0;
          j < spawnedEnemies && i < MAX_ENEMIES_ON_SCREEN;
          i++
        ) {
          if (enemies[i].spawned) {
            DrawRectangleRec( enemies[i].rect, enemies[i].color );
            j++;
          }
        }
        break;

      case WIN:
        DrawText(
          LABEL_WIN,
          ( screen.size.x - MeasureText(LABEL_WIN, screen.textSize) ) / 2,
          ( screen.size.y - screen.textSize ) / 2,
          screen.textSize, LIME
        );
        break;
      
      case OVERTIME:
        DrawText(
          LABEL_GAMEOVER,
          ( screen.size.x - MeasureText(LABEL_GAMEOVER, screen.textSize) ) / 2,
          ( screen.size.y - screen.textSize ) / 2,
          screen.textSize, screen.textColor
        );
        DrawText(
          LABEL_OVERTIME,
          ( screen.size.x - MeasureText(LABEL_OVERTIME, screen.textSize) ) / 2,
          ( screen.size.y + screen.textSize * 2 ) / 2,
          screen.textSize, RED
        );
        break;

      default:
        // Game Over (generic)
        DrawText(
          LABEL_GAMEOVER,
          ( screen.size.x - MeasureText(LABEL_GAMEOVER, screen.textSize) ) / 2,
          ( screen.size.y - screen.textSize ) / 2,
          screen.textSize, screen.textColor
        );
        break;
    }

    if ( gamestate != IN_GAME ) {
      // Retry (?)
      DrawText(
        LABEL_RETRY,
        ( screen.size.x - MeasureText(LABEL_RETRY, screen.textSize) ) / 2,
        screen.size.y - screen.textSize * 4,
        screen.textSize, GOLD
      );
      DrawText(
        LABEL_CHOISE,
        ( screen.size.x - MeasureText(LABEL_CHOISE, screen.textSize) ) / 2,
        screen.size.y - screen.textSize * 2.5,
        screen.textSize, BLACK
      );
    }

  EndDrawing();
}

void Unload(void) {  // Unload game variables
  // :Unload here all dynamic loaded data (textures, sounds, models...)
}