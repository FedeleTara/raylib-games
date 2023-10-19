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

#include "lib/enum.h"
#include "lib/struct.h"

#include <stdlib.h>
#include <math.h>

//------------------------------------------------------------------------------------
// Macros Definition
//------------------------------------------------------------------------------------
#define MAX_ENEMIES_ON_SCREEN 25
#define ENEMY_BLUE_TTS 5  // Time To Spawn

//------------------------------------------------------------------------------------
// Global Labels Declaration
//------------------------------------------------------------------------------------
const static char LABEL_TITLE[] = "--- [FRUITS CATCHER - Made with <3 and Raylib] ---";
const static char LABEL_GAMEOVER[] = "GAME OVER";
const static char LABEL_WIN[] = "YOU WIN";
const static char LABEL_OVERTIME[] = "OVERTIME";
const static char LABEL_RETRY[] = "RETRY?";
const static char LABEL_CHOISE[] = "Y (Yes) / N (No)";

//------------------------------------------------------------------------------------
// Global Constants Declaration
//------------------------------------------------------------------------------------
const static int SCREEN_WIDTH = 640;
const static int SCREEN_HEIGHT = 480;
const static Color SCREEN_COLOR = RAYWHITE;

const static int GAME_FPS = 60;
const static int MAX_TIME = 30;
const static int MAX_SCORE = 10;

const static Vector2 PLAYER_DIMENSION = { 57, 64 };
const static int PLAYER_STARTING_LIVES = 3;

const static Vector2 COLLECTABLES_DIMENSION = { 32, 32 };
const static double ENEMY_BLUE_TTM = 0.5;
const static double ENEMY_BLUE_VELOCITY = 3.75;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static int gameTime = 0;
static int spawnedEnemies = 0;
static int timeStart = 0;
static int timePassed = 0;
static int wave = 0;
static int score = 0;

static Font font = { 0 };
static Texture2D collectables_texture = { 0 };
static Texture2D players_texture = { 0 };

static GameState gamestate = IN_MENU;
static WindowDescription window = { 0 };
static Player player = { 0 };
static Enemy enemies[MAX_ENEMIES_ON_SCREEN] = { 0 };

//------------------------------------------------------------------------------------
// Functions Declaration
//------------------------------------------------------------------------------------
static void PlayerActionSpawn(void);
static void PlayerActionMove(void);

static void EnemyActionSpawn(Collectables type);
static bool EnemyActionFall(int index);
static void EnemyActionCollect(int index);

static void InitWindowDescription(void);
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
    
    // Free all the previous allocated memory
    Unload();

    // De-Initialization
    CloseWindow();    // Close window and OpenGL context

    return 0;
}

//------------------------------------------------------------------------------------
// Functions Definition
//------------------------------------------------------------------------------------

// Player Functions ------------------------------------------------------------------
void PlayerActionSpawn(void) {
  // TODO Play animation "Spawn Player"
  float pX = window.GameViewRect.width / 2 - PLAYER_DIMENSION.x;
  float pY = (window.GameViewRect.y + window.GameViewRect.height) -
    (PLAYER_DIMENSION.y + window.GameViewBorderSize + window.GameViewDeathLine);

  player.rect = (Rectangle) { pX, pY, PLAYER_DIMENSION.x, PLAYER_DIMENSION.y };
  player.lives = PLAYER_STARTING_LIVES;
  player.velocity = 4;
  player.color = RED;
}

void PlayerActionMove(void) {
  if (
      (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
      && player.rect.x > window.GameViewBorderSize + window.GameViewLRPadding
    ) {
    player.rect.x -= player.velocity;
  }

  if (
      (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
      && player.rect.x < (window.GameViewRect.width - window.GameViewLRPadding) - player.rect.width
    ) {
    player.rect.x += player.velocity;
  }
}

// Enemy (main) Functions -------------------------------------------------------------------
void EnemyActionSpawn(Collectables type) {
  for (int i = 0; i < MAX_ENEMIES_ON_SCREEN; i++) {
    if (!enemies[i].spawned) {
      if (type == CARROT) {
        // TODO Play animation "Spawn Blue"
        int eX = GetRandomValue(
          window.GameViewRect.y + window.GameViewLRPadding,
          window.GameViewRect.width - (COLLECTABLES_DIMENSION.x + window.GameViewLRPadding) 
        );
        int eY = window.GameViewRect.y + window.GameViewBorderSize;

        enemies[i].rect = (Rectangle) { eX, eY, COLLECTABLES_DIMENSION.x, COLLECTABLES_DIMENSION.y };
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
    ((window.GameViewRect.y + window.GameViewRect.height) - window.GameViewDeathLine) - enemies[index].rect.height
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
void InitWindowDescription(void) {
  int HUDTopHeight = SCREEN_HEIGHT / 10;
  int HUDBotHeight = SCREEN_HEIGHT / 12;
  int GameViewHeight = SCREEN_HEIGHT - (HUDTopHeight + HUDBotHeight);

  window.fontSize = 24;
  window.fontColor = BLACK;
  window.HUDBorderSize = 2;

  window.HUDTopRect = (Rectangle) { 0, 0, SCREEN_WIDTH, HUDTopHeight };
  window.HUDTopColor = GOLD;
  window.HUDTopBorderColor = DARKPURPLE;

  window.GameViewRect = (Rectangle) { 0, HUDTopHeight, SCREEN_WIDTH, SCREEN_HEIGHT - (HUDTopHeight + HUDBotHeight) };
  window.GameViewColor = SCREEN_COLOR;
  window.GameViewDeathLine = GameViewHeight / 100;
  window.GameViewLRPadding = PLAYER_DIMENSION.x / 10;
  window.GameViewBorderSize = 0;
  window.GameViewBorderColor = BLACK;

  window.HUDBotRect = (Rectangle) { 0, HUDTopHeight + GameViewHeight, SCREEN_WIDTH, HUDBotHeight };
  window.HUDBotColor = GOLD;
  window.HUDBotBorderColor = DARKPURPLE;
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
  // Window
  InitWindow( SCREEN_WIDTH, SCREEN_HEIGHT, LABEL_TITLE );
  SetTargetFPS( GAME_FPS );  // Set our game to run at 60 frames-per-second
  InitWindowDescription();

  // Font
  font = LoadFont("./fonts/Handmade.otf");
  GuiSetFont(font);
  GuiSetStyle(DEFAULT, TEXT_SIZE, window.fontSize);
  GuiSetStyle(DEFAULT, BORDER_WIDTH, window.HUDBorderSize + window.GameViewLRPadding);

  // Textures
  collectables_texture = LoadTexture("./sprite/collectables.png");
  players_texture = LoadTexture("./sprite/players.png");
  
  // Player
  PlayerActionSpawn();
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
        EnemyActionSpawn( CARROT );
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

    ClearBackground( SCREEN_COLOR );

    // HUD (Top) -----------------------------------------------------------------
    GuiDrawRectangle(
      window.HUDTopRect, window.HUDBorderSize,
      window.HUDTopBorderColor, window.HUDTopColor
    );
      GuiDrawText(
        TextFormat("Time %i", timePassed),
        GetTextBounds(DEFAULT, window.HUDTopRect),
        TEXT_ALIGN_LEFT, window.fontColor
      );
      GuiDrawText(
        TextFormat("Wave %i", wave),
        GetTextBounds(DEFAULT, window.HUDTopRect),
        TEXT_ALIGN_CENTER, window.fontColor
      );
      GuiDrawText(
        TextFormat("Score %i", score),
        GetTextBounds(DEFAULT, window.HUDTopRect),
        TEXT_ALIGN_RIGHT, window.fontColor
      );

    switch ( gamestate ) {
      /*case IN_MENU:
        GuiDrawRectangle(
          screen.infoBoxRect, screen.borderSize,
          screen.infoBoxBorderColor, screen.infoBoxColor
        );
        GuiDrawText(
          TextFormat("time %i", timePassed),
          GetTextBounds(DEFAULT, screen.infoBoxRect),
          GuiGetStyle(DEFAULT, TEXT_ALIGNMENT),
          BLACK
        );
        break;*/

      case IN_GAME:
        // GameView -------------------------------------------------------------
        GuiDrawRectangle(
          window.GameViewRect, window.GameViewBorderSize,
          window.GameViewBorderColor, window.GameViewColor
        );
          // Enemies --------------------------------------------------------------
          for (
            int i = 0, j = 0;
            j < spawnedEnemies && i < MAX_ENEMIES_ON_SCREEN;
            i++
          ) {
            if (enemies[i].spawned) {
              //DrawRectangleRec( enemies[i].rect, enemies[i].color );
              DrawTexturePro(
                collectables_texture,
                (Rectangle) { 96.0f, 0.0f, 32.0f, 32.0f },
                enemies[i].rect,
                (Vector2) { 0.0f, 0.0f },
                0.0f,
                RAYWHITE
              );
              j++;
            }
          }

          // Player ---------------------------------------------------------------
          DrawTexturePro(
            players_texture,
            (Rectangle) { 0.0f, 0.0f, 57.0f, 64.0f },
            player.rect,
            (Vector2) { 0.0f, 0.0f },
            0.0f,
            RAYWHITE
          );

        // HUD (Bot) -------------------------------------------------------------
        GuiDrawRectangle(
          window.HUDBotRect, window.HUDBorderSize,
          window.HUDBotBorderColor, window.HUDBotColor
        );
          GuiDrawText(
            TextFormat("Lives %i", player.lives),
            GetTextBounds(DEFAULT, window.HUDBotRect),
            TEXT_ALIGN_LEFT, window.fontColor
          );
        break;

      case WIN:
        DrawText(
          LABEL_WIN,
          ( SCREEN_WIDTH - MeasureText(LABEL_WIN, window.fontSize) ) / 2,
          ( SCREEN_HEIGHT - window.fontSize ) / 2,
          window.fontSize, LIME
        );
        break;
      
      case OVERTIME:
        DrawText(
          LABEL_GAMEOVER,
          ( SCREEN_WIDTH - MeasureText(LABEL_GAMEOVER, window.fontSize) ) / 2,
          ( SCREEN_HEIGHT - window.fontSize ) / 2,
          window.fontSize, window.fontColor
        );
        DrawText(
          LABEL_OVERTIME,
          ( SCREEN_WIDTH - MeasureText(LABEL_OVERTIME, window.fontSize) ) / 2,
          ( SCREEN_HEIGHT + window.fontSize * 2 ) / 2,
          window.fontSize, RED
        );
        break;

      default:
        // Game Over (generic)
        DrawText(
          LABEL_GAMEOVER,
          ( SCREEN_WIDTH - MeasureText(LABEL_GAMEOVER, window.fontSize) ) / 2,
          ( SCREEN_HEIGHT - window.fontSize ) / 2,
          window.fontSize, window.fontColor
        );
        break;
    }

    if ( gamestate != IN_GAME ) {
      // Retry (?)
      DrawText(
        LABEL_RETRY,
        ( SCREEN_WIDTH - MeasureText(LABEL_RETRY, window.fontSize) ) / 2,
        SCREEN_HEIGHT - window.fontSize * 4,
        window.fontSize, GOLD
      );
      DrawText(
        LABEL_CHOISE,
        ( SCREEN_WIDTH - MeasureText(LABEL_CHOISE, window.fontSize) ) / 2,
        SCREEN_HEIGHT - window.fontSize * 2.5,
        window.fontSize, BLACK
      );
    }

  EndDrawing();
}

void Unload(void) {  // Unload game variables
  // :Unload here all dynamic loaded data (textures, sounds, models...)
  UnloadFont(font);
  UnloadTexture(collectables_texture);
  UnloadTexture(players_texture);
}