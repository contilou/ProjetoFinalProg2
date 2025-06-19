#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include "map.h"

typedef struct {

    tMapPos matrixPos;
    Vector2 position;
    Vector2 direction;
    float speed;
    bool isAlive;

} Enemy;

typedef struct {

    Enemy *enemies;
    int count;

} EnemyGroup;

void InitEnemies(EnemyGroup *group, tMap *map);
void UpdateEnemies(EnemyGroup *group, float dt, tMap *map);
void DrawEnemies(EnemyGroup *group, tMap *map);
void FreeEnemies(EnemyGroup *group);

#endif