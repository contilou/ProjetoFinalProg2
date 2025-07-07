#ifndef BOX_H_
#define BOX_H_

#include <raylib.h>
#include "map.h"

typedef struct {

    tMapPos matrixPos;
    bool hasKey;
    bool destroyed;
    Texture2D box_sprite;

} tBox;

typedef struct {

    tMapPos matrixPos;
    bool unlocked;
    bool picked;
    Texture2D key_sprite;

} tKey;

typedef struct {

    tBox *boxes;
    int box_count;
    tKey *keys;
    int key_count;

} tBoxGroup;

void InitBoxes(tBoxGroup *group, tMap *map);
void DrawBoxes(tBoxGroup *group, tMap *map);
void DrawKeys(tBoxGroup *group, tMap *map);
void DestroyBox(tBoxGroup *group, tMapPos target_position, tMap *map);
void CheckKey(tBoxGroup *group, tMap *map);
void FreeBoxGroup(tBoxGroup *group);

#endif