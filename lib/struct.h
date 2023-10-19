//----------------------------------------------------------------------------------
// Structures Definition
//----------------------------------------------------------------------------------

#ifndef CUSTOM_STRUCT_H
  #define CUSTOM_STRUCT_H

  #include <raylib.h>
  #include "enum.h"

  /*
    [struct] WindowDescription
    (description) TODO
  */
  typedef struct {
    int fontSize;
    Color fontColor;
    int HUDBorderSize;

    Rectangle HUDTopRect;
    Color HUDTopColor;
    Color HUDTopBorderColor;

    Rectangle GameViewRect;
    Color GameViewColor;
    int GameViewDeathLine;
    int GameViewLRPadding;
    int GameViewBorderSize;
    Color GameViewBorderColor;

    Rectangle HUDBotRect;
    Color HUDBotColor;
    Color HUDBotBorderColor;
  } WindowDescription;

  /*
    [struct] Player
    (description) TODO
  */
  typedef struct {
    int lives;
    double velocity;

    Rectangle rect;
    Color color;
  } Player;

  /*
    [struct] Enemy
    (description) TODO
  */
  typedef struct {
    double ttm;
    double velocity;
    bool spawned;

    Collectables type;
    Rectangle rect;
    Color color;
  } Enemy;
#endif