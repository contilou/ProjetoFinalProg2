#ifndef BOX_H_
#define BOX_H_

#include <raylib.h>
#include "map.h"

typedef struct {

    tMapPos matrixPos;
    bool hasKey;
    bool destroyed;

} tBox;

typedef struct {

    tBox *boxes;
    int count;

} tBoxGroup;

void InitBoxes(tBoxGroup *group, tMap *map);
void DrawBoxes(tBoxGroup *group, tMap *map);

#endif