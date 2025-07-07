#include <raylib.h>
#include <stdlib.h>
#include "enemy.h"

void InitEnemies(EnemyGroup *group, tMap *map) {
    
    int totalEnemies = 0;
    Texture2D enemy_sprite_aux = LoadTexture("sprites/gotinha.png");
    // Conta quantos inimigos existem no mapa
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if (map->matrix[i][j] == 'E') totalEnemies++;
        }
    }

    // Aloca memória para os inimigos
    group->count = totalEnemies;
    group->enemies = malloc(sizeof(Enemy) * totalEnemies);

    // Inicializa os inimigos no mapa 
    int k = 0;
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if (map->matrix[i][j] == 'E') {
                Enemy *e = &group->enemies[k++];
                e->matrixPos.row    = i;
                e->matrixPos.column = j;
                e->screenPos.x      = j * map->tile_size;
                e->screenPos.y      = i * map->tile_size;
                e->speed            = 1.0f;
                e->timerDireção     = 0;
                map->matrix[i][j]   = ' ';
                e->enemy_sprite = enemy_sprite_aux; 
            }
        }
    }
}

void UpdateEnemies(EnemyGroup *group, float speed, tMap *map) {
    
    // Limpa todos os 'E's do mapa 
    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if (map->matrix[i][j] == 'E') map->matrix[i][j] = ' ';
        }
    }

    // Escolhe uma nova direção aleatoriamente
    for (int i = 0; i < group->count; i++) {
        Enemy *e = &group->enemies[i];

        if (e->timerDireção <= 0) {
            int d = rand() % 4;
            if      (d == 0) { e->direction = (Vector2){ 0, -1 }; }
            else if (d == 1) { e->direction = (Vector2){ 0,  1 }; }
            else if (d == 2) { e->direction = (Vector2){-1,  0 }; }
            else             { e->direction = (Vector2){ 1,  0 }; }
            e->timerDireção = 45 + (rand() % 45);
        }
        e->timerDireção--;

        // Nova posição 
        Vector2 newPos = {
            e->screenPos.x + e->direction.x * e->speed,
            e->screenPos.y + e->direction.y * e->speed
        };

        // Hitbox
        int tileSize = map->tile_size;
        int ex = (int)newPos.x;
        int ey = (int)newPos.y;
        int left   = ex / tileSize;
        int right  = (ex + tileSize - 1) / tileSize;
        int top    = ey / tileSize;
        int bottom = (ey + tileSize - 1) / tileSize;

        // Verificação dos espaços em volta do inimigo
        if (map->matrix[top][left] == ' ' &&
            map->matrix[top][right] == ' ' &&
            map->matrix[bottom][left] == ' ' &&
            map->matrix[bottom][right] == ' ') {

            e->screenPos = newPos;
            e->matrixPos.row = top;
            e->matrixPos.column = left;
        } else {
            e->timerDireção = 0;
        }
    }

    // Recoloca os 'E's no mapa
    for (int i = 0; i < group->count; i++) {
        Enemy *e = &group->enemies[i];
        map->matrix[e->matrixPos.row][e->matrixPos.column] = 'E';
    }
}

void DrawEnemies(EnemyGroup *group, tMap *map) {
    
    for (int i = 0; i < group->count; i++) {
        Enemy *e = &group->enemies[i];
        DrawTexture(e->enemy_sprite, e->screenPos.x, e->screenPos.y, WHITE);
    }
}

void FreeEnemies(EnemyGroup *group) {
    free(group->enemies);
    group->enemies = NULL;
    group->count   = 0;
}
