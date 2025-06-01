#include <raylib.h>
#include "player.h"

//Move o jogador utilizando WASD ou as setas do teclado
void MovePlayer(tPlayer *player){

    player->direction.x = (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT));
    player->direction.y = (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) - (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP));

    player->pos.x += player->direction.x * player->speed;
    player->pos.y += player->direction.y * player->speed;
}

//Desenha um frame do sprite do jogador
void DrawPlayer(tPlayer *player){

    DrawCircleV(player->pos, 10, BLACK);

}