#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include "map.h"

typedef struct {

    tMapPos matrixPos;
    Vector2 screenPos;
    Vector2 direction;
    float speed;
    float timerDireção;
    bool isAlive;

} Enemy;

typedef struct {

    Enemy *enemies;
    int count;
    Texture2D enemy_sprite;

} EnemyGroup;

void InitEnemies(EnemyGroup *group, tMap *map);
void UpdateEnemies(EnemyGroup *group, float dt, tMap *map);
void DrawEnemies(EnemyGroup *group, tMap *map);
void FreeEnemies(EnemyGroup *group);

#endif