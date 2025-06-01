#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "player.h"

//Abre o arquivo de texto com o mapa e o preenche na matriz do mapa
int GetMapMatrix(tMap *map){

    FILE* map_text;
    map_text = fopen(map->file_name, "r"); //Realiza a abertura do documento contendo o mapa em formato de texto

    //Verifica se o arquivo foi aberto corretamente
    if(map_text == NULL){
        return 0;
    }

    //Inicializa a matriz do mapa-------------
    map->matrix = (char**) malloc(map->rows * sizeof(char*));

    if(map->matrix == NULL){
        return -1;
    }

    for (int i = 0; i < map->rows; i++){

        *(map->matrix + i) = (char*) malloc(map->columns * sizeof(char));
        if(*(map->matrix + i) == NULL){
            return -1;
        }
    }
    //----------------------------------------

    //Percorre a matriz do mapa e preenche cada elemento com o caractere correspondente do arquivo,------
    for(int i = 0; i < map->rows; i++){

        for(int j = 0; j < map->columns; j++){

            char c = fgetc(map_text);
            while(c == '\n'){ //se o caractere obtido for uma quebrada de linha, ele será substituido pelo caractere seguinte até que a variável c tenha um caracter válido para a matriz
                c = fgetc(map_text);
            }
            map->matrix[i][j] = c;
        }
    }
    //---------------------------------------------------------------------------------------------------

    fclose(map_text); //Fecha o documento

    return 1;
}

//Percorre a matriz do mapa e desenha na tela uma parede para cada elemento que tiver 'W' (Deve ser utilizada na área de desenho na main.c

void DrawWalls(tMap* map){

    for(int i = 0; i < map->rows; i++){

        for(int j = 0; j < map->columns; j++){

            if (map->matrix[i][j] == 'W'){

                DrawRectangle(j * map->tile_size, i * map->tile_size, map->tile_size, map->tile_size, GRAY);

            }
        }
    }

 }