#include "box.h"
#include <stdlib.h>

int InitBoxes(tBoxGroup *group, tMap *map) {

    int total = 0;
    group->key_count = 0;
    Texture2D box_sprite_aux = LoadTexture("sprites/caixa.png");
    Texture2D key_sprite_aux = LoadTexture("sprites/chave.png");

    for(int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            switch(map->matrix[i][j]){
                case 'B':
                    total++;
                    break;

                case 'K':
                    group->key_count++;
                    total++;
                    break;
            }
        }
    }
    
    group->box_count = total;
    group->boxes = (tBox*) malloc(sizeof(tBox) * total);
    group->keys = (tKey*) malloc(sizeof(tKey) * group->key_count);

    // Verifica se a alocacao de memoria foi feita corretamente para as caixas
    if (group->boxes == NULL){
        return 0;
    }
    
    // Realiza a mesma verificacao de erro para as chaves 
    if (group->keys == NULL){
        return 0;
    }

    for(int i = 0; i < group->key_count; i++){
        group->keys[i].unlocked = false;
        group->key_sprite = key_sprite_aux;
    }

    int k = 0;
    for(int i = 0; i < map->rows; i++) {
        for (int j = 0; j < map->columns; j++) {
            if(map->matrix[i][j] == 'B' || map->matrix[i][j] == 'K') {
                tBox *currentBox = &group->boxes[k++];
                currentBox->matrixPos.row = i;
                currentBox->matrixPos.column = j;
                currentBox->destroyed = false;
                currentBox->hasKey = (bool) (map->matrix[i][j] == 'K');
                group->box_sprite = box_sprite_aux;

            }
        }
    }
    return 1;
}

void DrawBoxes(tBoxGroup *group, tMap *map){

    for(int i = 0; i < group->box_count; i++){
        tBox *currentBox = &group->boxes[i];
        if(!currentBox->destroyed) {
            DrawTexture(group->box_sprite, currentBox->matrixPos.column * map->tile_size, currentBox->matrixPos.row * map->tile_size, WHITE);
        }
    }
}

void DrawKeys(tBoxGroup *group, tMap *map){


    for(int i = 0; i < group->key_count; i++){
        tKey *currentKey = &group->keys[i];
        if(!currentKey->picked) {
            DrawTexture(group->key_sprite, currentKey->matrixPos.column * map->tile_size, currentKey->matrixPos.row * map->tile_size, WHITE);
        }

    }


}

void CheckKey(tBoxGroup *group, tMap *map){

    for(int i = 0; i < group->key_count; i++){
        tKey *currentKey = &group->keys[i];
        if(!group->keys[i].unlocked) continue;

        if(!currentKey->picked && (map->matrix[currentKey->matrixPos.row][currentKey->matrixPos.column] == ' ')) {
            currentKey->picked = true;
        }

    }

}

void DestroyBox(tBoxGroup *group, tMapPos target_position, tMap *map){

    for(int i = 0; i < group->box_count; i++){
        tBox *currentBox = &group->boxes[i];
        if(target_position.column == currentBox->matrixPos.column && target_position.row == currentBox->matrixPos.row){
            currentBox->destroyed = true;
            map->matrix[target_position.row][target_position.column] = ' ';
            if(currentBox->hasKey){
                map->matrix[currentBox->matrixPos.row][currentBox->matrixPos.column] = 'C';
                for(int i = 0; i < group->key_count; i++){
                    tKey *currentKey = &group->keys[i];
                    if(!currentKey->unlocked){

                        currentKey->unlocked = true;
                        currentKey->picked = false;
                        currentKey->matrixPos = (tMapPos){currentBox->matrixPos.row,currentBox->matrixPos.column};
                        break;
                        
                    }
                }
            }
        }
    }   
}

void FreeBoxGroup(tBoxGroup *group) {
    free(group->boxes);
    group->boxes = NULL;

    free(group->keys);
    group->keys = NULL;
}