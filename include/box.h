#ifndef BOX_H_
#define BOX_H_

#include <raylib.h>
#include <stdlib.h>
#include "map.h"

typedef struct {

    tMapPos matrixPos;
    bool hasKey;
    bool destroyed;

} tBox;

typedef struct {

    tMapPos matrixPos;
    bool unlocked;
    bool picked;
    

} tKey;

typedef struct {

    tBox *boxes;
    int box_count;
    Texture2D box_sprite;
    tKey *keys;
    int key_count;
    Texture2D key_sprite;

} tBoxGroup;

int InitBoxes(tBoxGroup *group, tMap *map);
void DrawBoxes(tBoxGroup *group, tMap *map);
void DrawKeys(tBoxGroup *group, tMap *map);
void DestroyBox(tBoxGroup *group, tMapPos target_position, tMap *map);
void CheckKey(tBoxGroup *group, tMap *map);
void FreeBoxGroup(tBoxGroup *group);

#endif