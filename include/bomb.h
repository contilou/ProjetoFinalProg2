#ifndef BOMB_H_
#define BOMB_H_
#include "map.h"
#include "player.h"
#include "sounds.h"

// salva as informações de cada uma das 3 bombas
typedef struct{
    int bombsLeft;
    bool isPlanted[3];
    bool exploded[3];
    double planted_times[3];
    double explosion_times[3];
    tMapPos positions[3];
    tMapPos area[3][9];
} tBomb;

void BombsManager(tPlayer *player, tMap *map, tBomb *bomb, AudioManager audio);
void PlantBomb(tBomb *bomb, tMap *mapa);
void resetBombInfo(tBomb *bomb);

#endif