#ifndef WALLD_H_
#define WALLD_H_

#include <raylib.h>
#include "map.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {

    tMapPos matrixPos;
    bool destroyed;
    

} tWallD;

typedef struct {

    tWallD *WallD;
    int WallD_count;
    Texture2D walld_sprite;
    
} tWallDGroup;

int InitWallD(tWallDGroup *group, tMap *map);
void DestroyWallD(tWallDGroup *group, tMapPos target_position, tMap *map);
void DrawWallsD(tWallDGroup *group, tMap *map);
void FreeWallD(tWallDGroup *group);

#endif