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

typedef enum GameScreen { MENU, GAMEPLAY, PAUSE} GameScreen;

//Variaveis locais
Camera2D camera = { 0 };
Vector2 circlePosition = { 0 };
tPlayer jogador = {{0,0}, {0,0}, {0,1}, true, 7, IDLE, 3, 0};
tMap mapa = {"mapa1.txt", NULL, 1, 25, 60, 20};
char texto[60], texto2[60], textobomba[10],textovida[10],textopont[30];
tBomb bomba = {3, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}};
EnemyGroup enemyGroup;
tBoxGroup boxGroup;
int started = 0;
double startPauseTime;

typedef struct{
    tMap map;
    tPlayer player;
    tBomb bomb;
  //  tBoxGroup boxGroup;
    EnemyGroup enemyGroup;
} tGameInfo;



//Funções locais
void UpdateDrawFrame(void);   // Atualiza e desenha um frame
int SaveGame();               // Salva o Jogo
int StartGame();              // começa o jogo do 0
int LoadGame();               // carrega os dados do último jogo nas variáveis locais 
void descontaTempo();         // função para descontar o tempo em que o jogo esteve pausada e as bombas, plantadas.


int main()
{
    //Altura e largura da janela
    const int screenWidth = 1200;
    const int screenHeight = 600;


    InitWindow(screenWidth, screenHeight, "raylib");
    ClearBackground(RAYWHITE);
    SetTargetFPS(60);               // Executa jogo para 60 frames por segundo

    if (!StartGame()) return 1;
    
    camera.target = (Vector2){0.0f, 0.0f};
    AudioManager audio = Carregasom();

    GameScreen currentScreen = MENU;

    while(!WindowShouldClose()){
        // Menu inicial
        if(currentScreen==MENU){
            if(IsKeyPressed(KEY_ENTER)){
                currentScreen=GAMEPLAY;
                started = 1;
            }
            if(IsKeyPressed(KEY_M)){
                // maiores pontuações devem ser mostradas
            }
            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawText("MENU PRINCIPAL", 470, 200, 30, WHITE);
            DrawRectangle(380,290,470,50, WHITE);
            DrawText("Jogar (Enter)", 500, 300, 30, DARKBLUE);
            DrawRectangle(380,360,470,50, WHITE);
            DrawText("Melhores Pontuações (M)", 420, 370, 30, DARKBLUE);
            EndDrawing();
        }
        // Tela de Pause
        else if(currentScreen==PAUSE){
            BeginDrawing();
            ClearBackground(DARKBLUE);
            // N apra iniciar novo jogo
            if (IsKeyPressed(KEY_N)){
                if (!StartGame()) return 1;
                descontaTempo();
                currentScreen = GAMEPLAY;    
            }
            // C para carregar jogo salvo
            if (IsKeyPressed(KEY_C)) {
                if(!LoadGame()){
                    BeginDrawing();
                    ClearBackground(DARKBLUE);
                    DrawText("Erro ao carregar jogo", 420, 300, 40, WHITE);
                    WaitTime(1);
                    EndDrawing();
                }
                descontaTempo();
                currentScreen = GAMEPLAY;
            }
            // S para salvar o progresso atul
            if (IsKeyPressed(KEY_S)) {
                if(SaveGame()){
                    BeginDrawing();
                    ClearBackground(DARKBLUE);
                    DrawText("Jogo salvo com sucesso", 370, 280, 40, WHITE);
                    EndDrawing();
                    WaitTime(1);
                }
                else{
                    BeginDrawing();
                    ClearBackground(DARKBLUE);
                    DrawText("Falha ao salvar jogo", 420, 300, 40, WHITE);
                    WaitTime(1);
                    EndDrawing();
                }
                currentScreen = PAUSE;
            }
            // Q para sair
            if (IsKeyPressed(KEY_Q)) break;
            // V para despausar
            if (IsKeyPressed(KEY_V)) {   
                descontaTempo();
                currentScreen = GAMEPLAY;
            }
            DrawText("Novo Jogo (N)", 440, 200, 30, WHITE);
            DrawText("Carregar Jogo (C)", 440, 230, 30, WHITE);
            DrawText("Salvar Jogo (S)", 440, 260, 30, WHITE);
            DrawText("Sair do Jogo (Q)", 440, 290, 30, WHITE);
            DrawText("Voltar (V)", 440, 320, 30, WHITE);
            EndDrawing();

        }
        // Gameplay
        else{
            if (IsKeyPressed(KEY_TAB)) {
                startPauseTime = GetTime();
                currentScreen=PAUSE;
            }
            ClearBackground(RAYWHITE);
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

            // na transição do menu para o jogo, há um intervalo de 0.7 segundos 
            if (started){
                WaitTime(0.3);
                started=0;
            }

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

// salva as informações do jogo atual. Retorna 0 caso um erro ocorra.
int SaveGame(){
    FILE *pfile;
    tGameInfo infos;
    infos.map = mapa;
    infos.player = jogador;
    infos.bomb = bomba;
    // infos.boxGroup = boxGroup;
    infos.enemyGroup = enemyGroup;
    if (!(pfile = fopen("lastGame.dat", "wb"))) return 0;
    fwrite(&infos, sizeof(tGameInfo), 1, pfile);
    fclose(pfile);
    return 1;
}

// Carrega o estado do jogo salvo no arquivo binário. Retorna 0 caso um erro ocorra.
int LoadGame(){
    FILE *pfile;
    tGameInfo infos;
    if (!(pfile = fopen("lastGame.dat", "rb"))) return 0;
    fread(&infos, sizeof(tGameInfo), 1, pfile);
    fclose(pfile);
    mapa = infos.map;
    jogador = infos.player;
    bomba = infos.bomb;
    //boxGroup = infos.boxGroup;
    enemyGroup = infos.enemyGroup;
    return 1;
}

// Inicia o jogo do 0. Retorna 0 caso um erro ocorra.
int StartGame(){
    resetBombInfo(&bomba);
    if(GetMapMatrix(&mapa) != 1){
        return 0;
    }
    GetPlayerStartPos(&jogador, &mapa);
    InitEnemies(&enemyGroup, &mapa);
    InitBoxes(&boxGroup, &mapa);
    camera.target = (Vector2){0.0f, 0.0f};
    return 1;
}

// se houvesse uma bomba plantada antes da pause, o tempo que
// o jogo ficou pausado deve ser descontado.
void descontaTempo(){
    for (int i=0; i<3; i++){
        if (bomba.isPlanted[i]==true){
            double delta = GetTime() - startPauseTime;
            bomba.planted_times[i] += delta;
            bomba.explosion_times[i] += delta;
        }
    }
}



