#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "map.h"
#include "bomb.h"
#include "sounds.h"
#include "enemy.h"
#include "box.h"

//Variaveis locais
Camera2D camera = { 0 };
Vector2 circlePosition = { 0 };
tPlayer jogador = {{0,0}, {0,0}, {0,1}, true, 7, IDLE, 3, 0};
tMap mapa = {"mapa1.txt", NULL, 1, 25, 60, 20};
char texto[60], texto2[60], textobomba[10],textovida[10],textopont[30];
tBomb bomba = {3, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}};
EnemyGroup enemyGroup;
tBoxGroup boxGroup;


//Funções locais
void UpdateDrawFrame(void);          // Atualiza e desenha um frame
int main()
{
    //Altura e largura da janela
    const int screenWidth = 1200;
    const int screenHeight = 600;
    
    if(GetMapMatrix(&mapa) != 1){

        return 1;

    }
    GetPlayerStartPos(&jogador, &mapa);
    InitEnemies(&enemyGroup, &mapa);
    InitBoxes(&boxGroup, &mapa);
    
    InitWindow(screenWidth, screenHeight, "raylib");        //Inicializa janela (tamanho e título)
    camera.target = (Vector2){0.0f, 0.0f};
    AudioManager audio = Carregasom();

    SetTargetFPS(60);               // Executa jogo para 60 frames por segundo


    // Game loop
    while (!WindowShouldClose())    // Detecta botão de fechamento da janela ou ESC
    {
        
        UpdateDrawFrame();
        BombsManager(&jogador, &mapa, &bomba, audio);
        MovePlayer(&jogador, &mapa); //Move o jogador
        float dt = GetFrameTime();
        UpdateEnemies(&enemyGroup, dt, &mapa);
        CheckKey(&boxGroup, &mapa);

        sprintf(texto, "Posição na tela - X: %d Y: %d", 20 * jogador.matrixPos.column, 20 * jogador.matrixPos.row); //Funciona com qlqr numeros de variaveis.
        sprintf(texto2, "Posição na matriz - Coluna: %d Linha: %d", jogador.matrixPos.column, jogador.matrixPos.row); //Dentro da variavel texto , ele põe outras variaveis
        sprintf(textobomba, "Bombas: %d", bomba.bombsLeft);   //Preencher apos criar o sistema de bombas
        sprintf(textovida, "Vidas: X");     //Preencher apos criar o sistema de vidas
        sprintf(textopont, "Pontuacao: XXX");   //Preencher apos criar o sistema de pontuacao

    //Bloco de código temporário, enquanto não fiz a interação da bomba com a caixa
        if(IsKeyPressed(KEY_P)){
            for(int i = 0; i < boxGroup.box_count; i++){
                tBox *currentBox = &boxGroup.boxes[i];
            if(!currentBox->destroyed){
                DestroyBox(&boxGroup,currentBox->matrixPos,&mapa);
            }

            }
        }
        
    }   

    FreeEnemies(&enemyGroup);
    Eliminasom(audio);
    CloseWindow();                  // Fecha a janela


    return 0;
}

// Atualiza os quadros do jogo
void UpdateDrawFrame(void)
{


    BeginDrawing();     //Inicia o ambiente de desenho na tela

        ClearBackground(SKYBLUE);   //Limpa a tela e define cor de fundo

        //BeginMode2D(camera);

            DrawWalls(&mapa);
            DrawKeys(&boxGroup, &mapa);
            DrawPlayer(&jogador, &mapa);  //Desenha o jogador 
            DrawEnemies(&enemyGroup, &mapa); //Desenha os inimigos
            DrawBoxes(&boxGroup, &mapa);
            

        //EndMode2D();

        DrawText(texto, 10, 15, 20, WHITE);
        DrawText(texto2, 350, 15, 20, WHITE);
        DrawText(textobomba, 20,500,20, BLACK);
        DrawText(textovida, 400,500,20, BLACK);
        DrawText(textopont, 800,500,20, BLACK);
    EndDrawing();   //Finaliza o ambiente de desenho na tela

}



