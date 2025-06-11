#ifndef BOMB_H_
#define BOMB_H_
#include "map.h"
#include "player.h"

typedef struct{
    int bombsLeft;
    bool isPlanted[3];
    double planted_times[3];
    Vector2 positions[3];
} tBomb;

void BombsManager(tPlayer *player, tMap *map, tBomb *bomb);

void PlantBomb(tBomb *bomb, tMap *mapa);









#endif