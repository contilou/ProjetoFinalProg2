#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

//Variaveis locais
Camera2D camera = { 0 };
Vector2 circlePosition = { 0 };
tPlayer bolinha = {{600,300}, {0,0}, 7};
char texto[60];
//Funções locais
void UpdateDrawFrame(void);          // Atualiza e desenha um frame


int main()
{
    //Altura e largura da janela
    const int screenWidth = 1200;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib");        //Inicializa janela (tamanho e título)

    camera.target = (Vector2){0.0f, 0.0f};


    SetTargetFPS(60);               // Executa jogo para 60 frames por segundo


    // Game loop
    while (!WindowShouldClose())    // Detecta botão de fechamento da janela ou ESC
    {
        UpdateDrawFrame();
        MovePlayer(&bolinha); //Move o jogador
        sprintf(texto, "X: %.2f Y: %.2f", bolinha.pos.x, bolinha.pos.y); //Funciona com qlqr numeros de variaveis.
    }                                                                  //Dentro da variavel texto , ele põe outras variaveis


    CloseWindow();                  // Fecha a janela


    return 0;
}

// Atualiza os quadros do jogo
void UpdateDrawFrame(void)
{


    BeginDrawing();     //Inicia o ambiente de desenho na tela

        ClearBackground(SKYBLUE);   //Limpa a tela e define cor de fundo

        //BeginMode2D(camera);

            DrawPlayer(&bolinha);  //Desenha o jogador 

        //EndMode2D();

        DrawText(texto, 10, 40, 20, DARKGRAY);

    EndDrawing();   //Finaliza o ambiente de desenho na tela

}



