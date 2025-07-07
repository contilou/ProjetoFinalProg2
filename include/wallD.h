#ifndef WALLD_H_
#define WALLD_H_

#include <raylib.h>
#include "map.h"

typedef struct {

    tMapPos matrixPos;
    bool destroyed;
    Texture2D walld_sprite;

} tWallD;

typedef struct {

    tWallD *WallD;
    int WallD_count;

} tWallDGroup;

void InitWallD(tWallDGroup *group, tMap *map);
void DestroyWallD(tWallDGroup *group, tMapPos target_position, tMap *map);
void DrawWallsD(tWallDGroup *group, tMap *map);
void FreeWallD(tWallDGroup *group);

#endif