#include "box.h"
#include <stdlib.h>

void InitBoxes(tBoxGroup *group, tMap *map) {

    int total = 0;
    
    for(int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if(map->matrix[i][j] == 'B' || map->matrix[i][j] == 'K') {
                total++;
            }
        }
    }
    
    group->count = total;
    group->boxes = (tBox*) malloc(sizeof(tBox) * total);

    int k = 0;
    for(int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if(map->matrix[i][j] == 'B' || map->matrix[i][j] == 'K') {
                tBox *currentBox = &group->boxes[k++];
                currentBox->matrixPos.row = i;
                currentBox->matrixPos.column = j;
                currentBox->destroyed = false;
                currentBox->hasKey = (bool) (map->matrix[i][j] == 'K');
            }
        }
    }
}

void DrawBoxes(tBoxGroup *group, tMap *map){


    for(int i = 0; i < group->count; i++){
        tBox *currentBox = &group->boxes[i];
        if(!currentBox->destroyed) {
            DrawRectangle(currentBox->matrixPos.column * map->tile_size, currentBox->matrixPos.row * map->tile_size, map->tile_size, map->tile_size, BROWN);
        }

    }


}