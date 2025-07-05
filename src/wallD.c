#include "wallD.h"
#include <stdio.h>
#include <stdlib.h>

void InitWallD(tWallDGroup *group, tMap *map){
    
    int total = 0;

    for (int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++)
        {
            if (map->matrix[i][j] == 'D')
            {
                total++;
            }
        }
    }
    
    group->WallD_count = total;
    group->WallD = (tWallD*) malloc(sizeof(tWallD) * total);

    if (group->WallD == NULL) {
        exit(EXIT_FAILURE); 
    }

    int k = 0;
    for(int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if(map->matrix[i][j] == 'D') {
                tWallD *currentWallD = &group->WallD[k++];
                currentWallD->matrixPos.row = i;
                currentWallD->matrixPos.column = j;
                currentWallD->destroyed = false;
            }
        }
    }
}

void DestroyWallD(tWallDGroup *group, tMapPos target_position, tMap *map){

    for(int i = 0; i < group->WallD_count; i++){
        tWallD *currentWallD = &group->WallD[i];
        if(target_position.column == currentWallD->matrixPos.column && target_position.row == currentWallD->matrixPos.row){
            currentWallD->destroyed = true;
            map->matrix[target_position.row][target_position.column] = ' ';
            break;
        }
    }
}


void FreeWallD(tWallDGroup *group) {
    free(group->WallD);
    group->WallD = NULL;
}
