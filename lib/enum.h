//----------------------------------------------------------------------------------
// Enums Definition
//----------------------------------------------------------------------------------

#ifndef CUSTOM_ENUM_H
  #define CUSTOM_ENUM_H

  /*
    [enum] Collectables
    (description) TODO
  */
  typedef enum {
    FIRE = 0,
    CARROT = 1,
    FANTASY_MANGO = 2,
    APPLE = 3,
  } Collectables;

  /*
    [enum] GameState
    (description) TODO
  */
  typedef enum {
    OVER = 0,
    IN_MENU = 1,
    IN_GAME = 2,
    OVERTIME = 3,
    WIN = 4,
  } GameState;
#endif