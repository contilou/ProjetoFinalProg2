#include <raylib.h>
#include "player.h"

void MovePlayer(tPlayer *player){

    player->direction.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    player->direction.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

    player->pos.x += player->direction.x * player->speed;
    player->pos.y += player->direction.y * player->speed;

}

void DrawPlayer(tPlayer *player){

    DrawCircleV(player->pos, 10, BLACK);

}
//TESTESTESTEST3EETSTESTESTE