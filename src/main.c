#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "player.h"
#include "map.h"
#include "bomb.h"
#include "sounds.h"
#include "enemy.h"
#include "box.h"
#include "wallD.h"

typedef enum GameScreen {MENU, GAMEPLAY, PAUSE} GameScreen;
typedef enum HUD {LEVEL, LIFE, BOMB, KEY, SCORE} HUD;

typedef struct{
    tMap map;
    tPlayer player;
    tBomb bomb;
    tBoxGroup boxGroup;
    EnemyGroup enemyGroup;
    tWallDGroup tWallDGroup;
} tGameInfo;

typedef struct{
    tMap map;
    tMap *maps;
    tPlayer player;
    tBomb bomb;
    tBoxGroup boxGroup;
    EnemyGroup enemyGroup;
    tWallDGroup wallDGroup;
    char texts[5][60];
    
} tElements;


//Funções locais
void UpdateDrawFrame(tElements *game_elements);   // Atualiza e desenha um frame
void checkExplosion(tElements *game_elements, int bombIndex, AudioManager audio);
int SaveGame(tElements *game_elements);               // Salva o Jogo
int StartGame(tElements *game_elements);              // começa o jogo do 0
int LoadGame(tElements *game_elements);               // carrega os dados do último jogo nas variáveis locais 
void descontaTempo(tBomb *bomb, double startPauseTime);         // função para descontar o tempo em que o jogo esteve pausada e as bombas, plantadas.
void ChangeMap(tElements *game_elements, AudioManager audio);

int main()
{
    //Variaveis locais
    tElements game_elements;
    game_elements.player = (tPlayer){{0,0}, {0,0}, {0,1}, true, 7, IDLE, 0, 3, 0, 3, false, 0.0f};
    game_elements.maps = NULL;
    int num_maps = 0;
    game_elements.bomb = (tBomb){3, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0}};
    int started = 0;
    double startPauseTime;
    AudioManager audio;

    //Altura e largura da janela
    const int screenWidth = 1200;
    const int screenHeight = 600;

    audio = Carregasom();

    InitWindow(screenWidth, screenHeight, "raylib");
    ClearBackground(RAYWHITE);
    SetTargetFPS(60);               // Executa jogo para 60 frames por segundo

    game_elements.bomb.sprite = LoadTexture("sprites/bomba.png");
    game_elements.bomb.explosion_tilemap = LoadTexture("sprites/bombaexplosao.png");

    if(InitMaps(&game_elements.maps, &num_maps) != 0){
        return 1;
    }
    game_elements.map = game_elements.maps[0];

    if (!StartGame(&game_elements)) return 1;
    Music musicamenu = LoadMusicStream("audio/musica.mp3");
    SetMusicVolume(musicamenu, 0.8);

    GameScreen currentScreen = MENU;

    while(!WindowShouldClose()){
        // Menu inicial
        if(currentScreen==MENU){
            UpdateMusicStream(audio.musicaMenu);
            if (!IsMusicStreamPlaying(audio.musicaMenu) && !IsSoundPlaying(audio.somMorte)) {
                PlayMusicStream(audio.musicaMenu);
            }
            if(IsKeyPressed(KEY_ENTER)){
                StopMusicStream(audio.musicaMenu);
                PlaySound(audio.somStart);
                currentScreen=GAMEPLAY;
                started = 1;
            }
            if(IsKeyPressed(KEY_M)){
                // maiores pontuações devem ser mostradas
            }
            BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawText("SUPER BOMBINHO ADVENTURES", 365, 200, 30, WHITE);
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
            UpdateMusicStream(audio.musicaPausa);
            if (IsMusicStreamPlaying(audio.musicaJogo)){
                PauseMusicStream(audio.musicaJogo);
            }
            
            if (!IsMusicStreamPlaying(audio.musicaPausa)){
                PlayMusicStream(audio.musicaPausa);
            }
            // N apra iniciar novo jogo
            if (IsKeyPressed(KEY_N)){
                StopMusicStream(audio.musicaPausa);
                StopMusicStream(audio.musicaJogo);
                PlaySound(audio.somStart);
                if(InitMaps(&game_elements.maps, &num_maps) != 0){
                    return 1;
                }
                game_elements.player.score = 0;
                game_elements.map = game_elements.maps[0];
                if (!StartGame(&game_elements)) return 1;
                descontaTempo(&game_elements.bomb, startPauseTime);
                currentScreen = GAMEPLAY;    
            }
            // C para carregar jogo salvo
            if (IsKeyPressed(KEY_C)) {
                StopMusicStream(audio.musicaPausa);
                if(!LoadGame(&game_elements)){
                    BeginDrawing();
                    ClearBackground(DARKBLUE);
                    DrawText("Erro ao carregar jogo", 420, 300, 40, WHITE);
                    WaitTime(1);
                    EndDrawing();
                }
                descontaTempo(&game_elements.bomb, startPauseTime);
                currentScreen = GAMEPLAY;
            }
            // S para salvar o progresso atul
            if (IsKeyPressed(KEY_S)) {
                StopMusicStream(audio.musicaPausa);
                PlaySound(audio.somBotao);
                if(SaveGame(&game_elements)){
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
            if (IsKeyPressed(KEY_Q)){
                StopMusicStream(audio.musicaPausa);
                break;
            }
            // V para despausar
            if (IsKeyPressed(KEY_V)) {   
                if (IsMusicStreamPlaying(audio.musicaJogo)){
                ResumeMusicStream(audio.musicaJogo); 
                }
                descontaTempo(&game_elements.bomb, startPauseTime);
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
            UpdateMusicStream(audio.musicaJogo);
            if (!IsMusicStreamPlaying(audio.musicaJogo)){
                PlayMusicStream(audio.musicaJogo);
            }
            if (IsKeyPressed(KEY_TAB)) {
                PauseMusicStream(audio.musicaJogo);
                UpdateMusicStream(audio.musicaPausa);
                startPauseTime = GetTime();
                currentScreen=PAUSE;
            }
            ClearBackground(RAYWHITE);
            UpdateDrawFrame(&game_elements);
            BombsManager(&game_elements.player, &game_elements.map, &game_elements.bomb, audio);
            if(game_elements.player.keys >= 5){
                ChangeMap(&game_elements, audio);//Passa de nível quando o jogador pega 5 chaves
            }
            MovePlayer(&game_elements.player, &game_elements.map, audio); //Move o jogador
            float dt = GetFrameTime();
            UpdateEnemies(&game_elements.enemyGroup, dt, &game_elements.map);
            CheckKey(&game_elements.boxGroup, &game_elements.map);

            sprintf(game_elements.texts[BOMB], "Bombas: %d", game_elements.bomb.bombsLeft);   //Indicador de bombas restantes
            sprintf(game_elements.texts[LIFE], "Vidas: %d", game_elements.player.lives);     //Preencher apos criar o sistema de vidas
            sprintf(game_elements.texts[SCORE], "Pontuacao: %d", game_elements.player.score);   //Indicador de posição
            sprintf(game_elements.texts[LEVEL], "Nivel %d", game_elements.map.mapId);
            sprintf(game_elements.texts[KEY], "Chaves: %d/5", game_elements.player.keys); // Subsituir pelo sprite da chave dps

            for(int i = 0; i < 3; i++){
                if(game_elements.bomb.exploded[i]){
                    checkExplosion(&game_elements, i, audio);
                }
            }
            
            //Se o jogador morrer
            if (game_elements.player.lives <= 0) {
                //Retorna ao menu
                PlaySound(audio.somMorte);
                currentScreen = MENU;
                // Reinicia o estado do jogo para quando o jogador quiser jogar novamente
                if(InitMaps(&game_elements.maps, &num_maps) != 0){
                    return 1;
                }
                game_elements.map = game_elements.maps[0];

                if (!StartGame(&game_elements)) return 1;
            }

            // na transição do menu para o jogo, há um intervalo de 0.7 segundos 
            if (started){
                WaitTime(0.3);
                started=0;
            }

            if(DamageByEnemies(&game_elements.enemyGroup, &game_elements.player)){
                DamagePlayer(&game_elements.player, audio);
            }
            

            //Bloco de código temporário, enquanto não fiz a interação da bomba com a Parede destrutivel
            if(IsKeyPressed(KEY_P)){
                for(int i = 0; i < game_elements.wallDGroup.WallD_count; i++){
                    tWallD *currentWallD = &game_elements.wallDGroup.WallD[i];
                    if(!currentWallD->destroyed){
                        DestroyWallD(&game_elements.wallDGroup,currentWallD->matrixPos,&game_elements.map);
                    }
                }
            }
        } 
    }


    UnloadTexture(game_elements.map.wall_sprite);
    UnloadTexture(game_elements.maps->wall_sprite);
    UnloadTexture(game_elements.wallDGroup.walld_sprite);
    UnloadTexture(game_elements.boxGroup.box_sprite);
    UnloadTexture(game_elements.boxGroup.key_sprite);
    UnloadTexture(game_elements.enemyGroup.enemy_sprite);
    UnloadTexture(game_elements.bomb.sprite);
    UnloadTexture(game_elements.bomb.explosion_tilemap);
    FreeBoxGroup(&game_elements.boxGroup);
    FreeWallD(&game_elements.wallDGroup);
    FreeEnemies(&game_elements.enemyGroup);
    EliminaAudio(audio);
    UnloadMusicStream(musicamenu);
    CloseWindow();                  // Fecha a janela


    return 0;
}

// Atualiza os quadros do jogo
void UpdateDrawFrame(tElements *game_elements)
{

    BeginDrawing();     //Inicia o ambiente de desenho na tela
        Color background = GetColor(0x9BD5FF00);
        ClearBackground(background);   //Limpa a tela e define cor de fundo

        //BeginMode2D(camera);

            DrawWalls(&game_elements->map);
            DrawWallsD(&game_elements->wallDGroup, &game_elements->map);
            DrawKeys(&game_elements->boxGroup, &game_elements->map);
            DrawPlayer(&game_elements->player, &game_elements->map);  //Desenha o jogador 
            DrawEnemies(&game_elements->enemyGroup, &game_elements->map); //Desenha os inimigos
            DrawBoxes(&game_elements->boxGroup, &game_elements->map);            

        //EndMode2D();

        DrawText(game_elements->texts[LEVEL], 70,540,20, BLACK);
        DrawText(game_elements->texts[LIFE], 302,540,20, BLACK);
        DrawText(game_elements->texts[BOMB], 534,540,20, BLACK);
        DrawText(game_elements->texts[SCORE], 766,540,20, BLACK);
        DrawText(game_elements->texts[KEY], 998,540,20, BLACK);
    EndDrawing();   //Finaliza o ambiente de desenho na tela

}

void checkExplosion(tElements *game_elements, int bombIndex, AudioManager audio){
    for(int i=0;i<9;i++){

        tMapPos explosionPos = {game_elements->bomb.area[bombIndex][i].row,game_elements->bomb.area[bombIndex][i].column};

        if(explosionPos.row == -1){
            continue;
        }

        //Se a explosão atingiu o jogador, aplica o dano.
        if (explosionPos.row == game_elements->player.matrixPos.row && explosionPos.column == game_elements->player.matrixPos.column) {
            DamagePlayer(&game_elements->player, audio);
            // Não é necessário continuar verificando esta explosão, pois o dano já foi aplicado
            // e o jogador está invencível por um curto período.
        }

        for (int e = 0; e < game_elements->enemyGroup.count; e++) {
            if (!game_elements->enemyGroup.enemies[e].isAlive) continue;
            if (game_elements->enemyGroup.enemies[e].matrixPos.row == explosionPos.row &&
                game_elements->enemyGroup.enemies[e].matrixPos.column == explosionPos.column) {
                game_elements->enemyGroup.enemies[e].isAlive = false;
                break; 
            }
        }

        switch (game_elements->map.matrix[explosionPos.row][explosionPos.column]){
            case 'K':
            case 'B':
                DestroyBox(&game_elements->boxGroup, explosionPos, &game_elements->map);
                ChangeScore(&game_elements->player, 10);
                break;
            case 'C':
                break;
            case 'D':
                DestroyWallD(&game_elements->wallDGroup, explosionPos, &game_elements->map);
                ChangeScore(&game_elements->player, 10);
                break;
            case 'E':
                ChangeScore(&game_elements->player, 20);
                break;
        }

    }
}

// salva as informações do jogo atual. Retorna 0 caso um erro ocorra.
int SaveGame(tElements *game_elements){
    FILE *pfile;
    tGameInfo infos;
    if (!(pfile = fopen("lastGame.dat", "wb"))) return 0;

    fwrite(&game_elements->player, sizeof(tPlayer), 1, pfile);
    fwrite(&game_elements->bomb, sizeof(tBomb), 1, pfile);


    fwrite(&game_elements->boxGroup.box_count, sizeof(int), 1, pfile);
    fwrite(game_elements->boxGroup.boxes, sizeof(tBox), game_elements->boxGroup.box_count, pfile);
    fwrite(&game_elements->boxGroup.key_count, sizeof(int), 1, pfile);
    fwrite(game_elements->boxGroup.keys, sizeof(tKey), game_elements->boxGroup.key_count, pfile);


    fwrite(&game_elements->wallDGroup.WallD_count, sizeof(int), 1, pfile);
    fwrite(game_elements->wallDGroup.WallD, sizeof(tWallD), game_elements->wallDGroup.WallD_count, pfile);

    fwrite(&game_elements->enemyGroup.count, sizeof(int), 1, pfile);
    fwrite(game_elements->enemyGroup.enemies, sizeof(Enemy), game_elements->enemyGroup.count, pfile);

    fwrite(&game_elements->map.rows, sizeof(int), 1, pfile);
    fwrite(&game_elements->map.columns, sizeof(int), 1, pfile);
    fwrite(&game_elements->map.mapId, sizeof(int), 1, pfile);
    for (int i = 0; i < game_elements->map.rows; i++) {
        fwrite(game_elements->map.matrix[i], sizeof(char), game_elements->map.columns, pfile);
    }


    fclose(pfile);
    return 1;
}

// Carrega o estado do jogo salvo no arquivo binário. Retorna 0 caso um erro ocorra.
int LoadGame(tElements *game_elements){
    FILE *pfile;
    tGameInfo infos;
    if (!(pfile = fopen("lastGame.dat", "rb"))) return 0;
    
    FreeBoxGroup(&game_elements->boxGroup);
    FreeWallD(&game_elements->wallDGroup);
    FreeEnemies(&game_elements->enemyGroup);

    if (!StartGame(game_elements)) return 1;

    //Leitura dos dados do player
    fread(&game_elements->player, sizeof(tPlayer), 1, pfile);

    //Leitura dos dados da bomba
    fread(&game_elements->bomb, sizeof(tBomb), 1, pfile);

    //Leitura dos dados da caixa
    fread(&game_elements->boxGroup.box_count, sizeof(int), 1, pfile);
    game_elements->boxGroup.boxes = malloc(sizeof(tBox) * game_elements->boxGroup.box_count);
    fread(game_elements->boxGroup.boxes, sizeof(tBox), game_elements->boxGroup.box_count, pfile);
    fread(&game_elements->boxGroup.key_count, sizeof(int), 1, pfile);
    game_elements->boxGroup.keys = malloc(sizeof(tKey) * game_elements->boxGroup.key_count);
    fread(game_elements->boxGroup.keys, sizeof(tKey), game_elements->boxGroup.key_count, pfile);

    //Leitura dos dados da parede destrutivavel
    fread(&game_elements->wallDGroup.WallD_count, sizeof(int), 1, pfile);
    game_elements->wallDGroup.WallD = malloc(sizeof(tWallD) * game_elements->wallDGroup.WallD_count);
    fread(game_elements->wallDGroup.WallD, sizeof(tWallD), game_elements->wallDGroup.WallD_count, pfile);

    //Leitura dos dados do inimigo
    fread(&game_elements->enemyGroup.count, sizeof(int), 1, pfile);
    game_elements->enemyGroup.enemies = malloc(sizeof(Enemy) * game_elements->enemyGroup.count);
    fread(game_elements->enemyGroup.enemies, sizeof(Enemy), game_elements->enemyGroup.count, pfile);

    //Leitura dos dados do mapa
    fread(&game_elements->map.rows, sizeof(int), 1, pfile);
    fread(&game_elements->map.columns, sizeof(int), 1, pfile);
    fread(&game_elements->map.mapId, sizeof(int), 1, pfile);
    game_elements->map.matrix = malloc(game_elements->map.rows * sizeof(char*));
    for (int i = 0; i < game_elements->map.rows; i++) {
        game_elements->map.matrix[i] = malloc(game_elements->map.columns * sizeof(char));
        fread(game_elements->map.matrix[i], sizeof(char), game_elements->map.columns, pfile);
    }

    game_elements->player.player_sprite = LoadTexture("sprites/gotinha.png"); 
    game_elements->boxGroup.box_sprite = LoadTexture("sprites/caixa.png");
    game_elements->boxGroup.key_sprite = LoadTexture("sprites/chave.png");
    game_elements->wallDGroup.walld_sprite = LoadTexture("sprites/parededestrutivel.png"); 
    game_elements->enemyGroup.enemy_sprite = LoadTexture("sprites/gotinha.png");
    game_elements->map.wall_sprite = LoadTexture("sprites/parede.png");

    fclose(pfile);

    return 1;
}

// Inicia o jogo do 0. Retorna 0 caso um erro ocorra.
int StartGame(tElements *game_elements){
    resetBombInfo(&game_elements->bomb);
    int score = game_elements->player.score;
    game_elements->player = (tPlayer){{0,0}, {0,0}, {0,1}, true, 7, IDLE, 0, 3, 0, 3, false, 0.0f};
    game_elements->player.lives = 3;
    game_elements->player.score = score;
    game_elements->player.is_invincible = false;
    game_elements->player.invincibility_timer = 0;
    GetPlayerStartPos(&game_elements->player, &game_elements->map);
    
    // Se ocorrer um erro em qualquer uma das inicializacoes a funcao retornara 0
    if ((InitEnemies(&game_elements->enemyGroup, &game_elements->map)) !=1 ||
    (InitBoxes(&game_elements->boxGroup, &game_elements->map)) !=1 ||
    (InitWallD(&game_elements->wallDGroup, &game_elements->map)) !=1) {
       return 0;
    }
    return 1;
}

// se houvesse uma bomba plantada antes da pause, o tempo que
// o jogo ficou pausado deve ser descontado.
void descontaTempo(tBomb *bomb, double startPauseTime){
    for (int i=0; i<3; i++){
        if (bomb->isPlanted[i]==true){
            double delta = GetTime() - startPauseTime;
            bomb->planted_times[i] += delta;
            bomb->explosion_times[i] += delta;
        }
    }
}

void ChangeMap(tElements *game_elements, AudioManager audio){
    int next_map_index = game_elements->map.mapId;
    PlaySound(audio.somNext_Map);
    WaitTime(2.6);
    game_elements->map = game_elements->maps[next_map_index];

    StartGame(game_elements);

}