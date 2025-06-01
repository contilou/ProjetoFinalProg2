/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

//----------------------------------------------------------------------------------
// Local Variables Definition (local to this module)
//----------------------------------------------------------------------------------
Camera2D camera = { 0 };
Vector2 circlePosition = { 0 };
tPlayer bolinha = {{600,300}, {0,0}, 7};
char texto[60];
//----------------------------------------------------------------------------------
// Local Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame(void);          // Update and draw one frame



//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1200;
    const int screenHeight = 600;
    strcpy(texto, "oi");

    InitWindow(screenWidth, screenHeight, "raylib");        //Inicializa janela (tamanho e título)

    camera.target = (Vector2){0.0f, 0.0f};

    //--------------------------------------------------------------------------------------


    SetTargetFPS(60);               // Ajusta a execução do jogo para 60 frames por segundo
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detecta botão de fechamento da janela ou ESC
    {
        UpdateDrawFrame();
        MovePlayer(&bolinha);
        sprintf(texto, "X: %.2f Y: %.2f abobora Leiga", bolinha.pos.x, bolinha.pos.y);
    }


    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Fecha a janela e o contexto OpenGL
    //--------------------------------------------------------------------------------------

    return 0;
}

// Update and draw game frame
void UpdateDrawFrame(void)
{
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();     //Inicia o ambiente de desenho na tela

        ClearBackground(SKYBLUE);   //Limpa a tela e define cor de fundo

        //BeginMode2D(camera);

            DrawPlayer(&bolinha);   

        //EndMode2D();

        DrawText(texto, 10, 40, 20, DARKGRAY);  //Desenha texto, posição, tamanho e cor

    EndDrawing();   //Finaliza o ambiente de desenho na tela
    //----------------------------------------------------------------------------------
}



