#include <raylib.h>
#include <stdlib.h>
#include "enemy.h"

void InitEnemies(EnemyGroup *group, tMap *map) {

    int total = 0;
    
    //Lê a quantidade de inimigos no mapa
    for(int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if(map->matrix[i][j] == 'E') {
                total++;
            }
        }
    }
    
    //Aloca dinamicamente a memória para os inimigos
    group->count = total;
    group->enemies = malloc(sizeof(Enemy) * total);

    //Inicializa os inimigos
    int k = 0;
    for(int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if(map->matrix[i][j] == 'E') {
                Enemy *e = &group->enemies[k++];
                e->matrixPos.row = i;
                e->matrixPos.column = j;
                e->position = (Vector2){j * map->tile_size, i * map->tile_size};
                e->direction = (Vector2){1, 0};
                e->speed = 60.0f;
                e->isAlive = true;
            }
        }
    }
}  

void UpdateEnemies(EnemyGroup *group, float dt, tMap *map) {

    //Verifica se cada inimigo continua vivo
    for (int i = 0; i < group->count; i++) {
        Enemy *e = &group->enemies[i];
        if(!e->isAlive) continue;
    
        Vector2 newPos = {
            e->position.x + e->direction.x * e->speed * dt,
            e->position.y + e->direction.y * e->speed * dt
        };
   
        //Converte a posição para um índice na matriz
        int tileX = (int)(newPos.x / map->tile_size);
        int tileY = (int) (newPos.y / map ->tile_size);
    
        //Verifica se há algum obstáculo
        if(tileX >= 0 && tileX < map->columns && tileY >= 0 && tileY < map->rows) {
            if(map->matrix[tileY][tileX] == ' ') {
                e->position = newPos;
                e->matrixPos.row = tileY;
                e->matrixPos.column = tileX;
            }
            else {
                //Muda de direção caso haja um obstáculo
                int dx = 0, dy = 0;
                while (dx == 0 && dy == 0) {
                    dx = (rand() % 3) - 1; // -1, 0 ou 1
                    dy = (rand() % 3) - 1;
                    if (dx != 0 && dy != 0) {  // impede diagonal
                        if (rand() % 2) dx = 0;
                            else dy = 0;
                    }
                }
        e->direction.x = dx;
        e->direction.y = dy;
    }
        }
    }
}

void DrawEnemies(EnemyGroup *group, tMap *map) {
    //Verifica se cada inimigo está vivo e desenha no mapa
    for(int i = 0; i < group->count; i++) {
        if(group->enemies[i].isAlive) {
            DrawRectangleV(group->enemies[i].position, (Vector2){map->tile_size, map->tile_size}, RED);
        }
    }
}

void FreeEnemies(EnemyGroup *group) {
    free(group->enemies);
    group->enemies = NULL;
    group->count = 0;
}
