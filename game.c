// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

// Hint: To run every line, just press F6 (on Notepad++) or F5 (on VS Code) (if you're using another solution, idk)

//------------------------------------------------------------------------------------
// Data Structures
struct LinkedList {
    Vector2 pos;
    struct LinkedList *next;
};

typedef struct LinkedList *node;

node createNode(Vector2 pos) {
    node tmp = (node) malloc( sizeof(struct LinkedList) );
    tmp->pos = pos;
    tmp->next = NULL;
    return tmp;
}

node addNode(node head, Vector2 pos) {
    node tmp, p;
    tmp = createNode(pos);

    if ( head == NULL ) {
        head = tmp;
        return head;
    }

    p = head;
    while ( p->next != NULL )
        p = p->next;
    p->next = tmp;

    return head;
}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------
    const int screenWidth = 350;
    const int screenHeight = 450;
    
    const int marioWidth = 25;
    const int marioHeight = 40;

    char msg[100] = "Click pls";

    Vector2 posMario = { 0, 0 };
    Vector2 posGoomba = {
        GetRandomValue(marioWidth + 1, screenWidth - marioWidth),
        GetRandomValue(marioHeight + 1, screenHeight - marioHeight)
    };
    Vector2 posGoomba2 = {
        GetRandomValue(marioWidth + 1, screenWidth - marioWidth),
        GetRandomValue(marioHeight + 1, screenHeight - marioHeight)
    };
    Vector2 posGoomba3;
    node posGoombas = NULL;
    
    InitWindow(screenWidth, screenHeight, "--- [Raylib - First Attempt] ---");
    SetTargetFPS(60);    // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------
    // Main game loop
    while(!WindowShouldClose()) {    // Detect window close button or ESC key
        // Update
        posGoomba3 = GetMousePosition();
        
        if ( (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) && posMario.x < screenWidth - marioWidth )
            posMario.x += 4;

        if ( (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) && posMario.x > 0 )
            posMario.x -= 4;
        
        if ( (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && posMario.y < screenHeight - marioHeight )
            posMario.y += 4;
        
        if ( (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) && posMario.y > 0 )
            posMario.y -= 4;
        
        if ( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ) {
            posGoombas = addNode(posGoombas, posGoomba3);
            sprintf(msg, "%.2f, %.2f", posGoomba3.x, posGoomba3.y);
        }
        //----------------------------------------------------------------------------
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangle(posMario.x, posMario.y, marioWidth, marioHeight, RED);
            DrawRectangle(posGoomba.x, posGoomba.y, marioWidth, marioHeight, BLUE);
            DrawRectangle(posGoomba2.x, posGoomba2.y, marioWidth, marioHeight, ORANGE);
            DrawRectangle(posGoomba3.x, posGoomba3.y, marioWidth, marioHeight, PINK);
            DrawText(msg, 125, 150, 20, BLACK);
        EndDrawing();
        //----------------------------------------------------------------------------
    }
    // De-Initialization
    //--------------------------------------------------------------------------------
    CloseWindow();    // Close window and OpenGL context
    //--------------------------------------------------------------------------------
    
    return 0;
}