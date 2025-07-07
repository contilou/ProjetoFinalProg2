#ifndef PLAYER_H_
#define PLAYER_H_

#include <raylib.h>
#include "map.h"
#include "enemy.h"

typedef enum {IDLE, MOVING} pState; //Declara um enum com estados possíveis para o jogador


//Struct que define as propriedades do jogador
typedef struct {

    tMapPos matrixPos; //Struct que armazena a posição do jogador na matriz do mapa
    Vector2 screenPos; //Posição visual do jogador (utilizando os pixels totais da tela como base)
    Vector2 direction; //Vetor para a direção que jogador está olhando (x,y)
    bool horizontal_priority;

    float speed; //Velocidade do jogador
    pState state; //Estado do jogador

    int score; //Pontuação
    int bombsLeft; //Bombas restantes
    
    int keys; //Chaves obtidas pelo jogador
    int lives;
    bool is_invincible;
    float invincibility_timer;
    Texture2D player_sprite;

} tPlayer;



//Declaração das funções
void GetPlayerStartPos(tPlayer *player, tMap *map);
void MovePlayer(tPlayer *player, tMap* map);
void DrawPlayer(tPlayer *player, tMap* map);
void ChangeScore(tPlayer *player, int score);
bool isElementSolid(char elem);
void DamagePlayer(tPlayer *player);
bool DamageByEnemies(EnemyGroup *group, tPlayer *player);

#endif
