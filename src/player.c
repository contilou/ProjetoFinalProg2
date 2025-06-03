#include <raylib.h>
#include <math.h>
#include "player.h"

//Percorre a matriz do mapa e, após achar a posição inicial do jogador, armazenas na propriedades matrixPos e screenPos
void GetPlayerStartPos(tPlayer *player, tMap *map){

    for(int i = 0; i < map->rows; i++){

            for(int j = 0; j < map->columns; j++){

                if (map->matrix[i][j] == 'J'){

                    //Atribuição dos valores referentes à posição na matriz do mapa
                    player->matrixPos.row = i;
                    player->matrixPos.column = j;

                    //Atribuição dos valores referentes à posição na tela do jogo
                    player->screenPos.x = j * map->tile_size;
                    player->screenPos.y = i * map->tile_size;
                }
            }
        }

}

//Move o jogador utilizando WASD ou as setas do teclado
void MovePlayer(tPlayer *player, tMap *map){

    //Roda apenas se o jogador não estiver se movendo
    if(player->state == IDLE){

        //Corresponde a direção de movimento (baseada nas teclas que o usúario pressiona no instante que a função foi chamada) 
        Vector2 move_direction;
        //Se o jogador apertar o botão para esquerda (Tecla A ou seta esquerda), a direção horizontal vai ser -1, caso seja para direita (Tecla D ou seta direita), vai ser 1
        move_direction.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
        //Se o jogador apertar o botão para baixo (Tecla S ou seta baixo), a direção vertical vai ser 1, caso seja para cima (Tecla W ou seta cima), vai ser -1
        move_direction.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);

        //Verifica se a direção de movimento vertical é diferente de 0
        if(move_direction.y){
            player->direction = (Vector2){0,move_direction.y}; //Atualiza a direção de onde o personagem está olhando como um vetor vertical (para cima ou para baixo);
            player->matrixPos.row += player->direction.y; //Atualiza a posição vertical (matriz) do jogador
            player->state = MOVING;
        }

        //Verifica se a direção de movimento horizontal é diferente de 0
        if(move_direction.x){
            player->direction = (Vector2){move_direction.x,0}; //Atualiza a direção de onde o personagem está olhando como um vetor horizontal (para esquerda ou para direita);
            player->matrixPos.column += player->direction.x; //Atualiza a posição horizontal (matriz) do jogador
            player->state = MOVING;
        }

    //Se o jogador já estiver no estado MOVING,ou seja, a posição já matriz já tenha sido alterada), a posição do jogador na tela é alterada gradualmente em cada chamada da função
    //trazendo um efeito de suavização
    } else if (player->state == MOVING){

        //Posições alvos que o jogador deverá alcançar na tela 
        float goalPosX = map->tile_size * player->matrixPos.column; 
        float goalPosY = map->tile_size * player->matrixPos.row;

        //Atualiza as posições x e y do jogador na tela de maneira suave (as posições são incrementadas com base na diferença entre a posição alvo e a posição atual);
        player->screenPos.x += (goalPosX - player->screenPos.x)/2.5;
        player->screenPos.y += (goalPosY - player->screenPos.y)/2.5;

        //Se a diferença entre a posição alvo e atual nos dois eixos forem ambas menores que 2, o valor da posição alvo é atribuido ao valor da posição atual
        //Após isso, o jogador retorna ao estado parado (IDLE)
        if((fabs(goalPosX - player->screenPos.x) < 2) && (fabs(goalPosY - player->screenPos.y) < 2)){
            player->screenPos.x = goalPosX;
            player->screenPos.y = goalPosY;
            player->state = IDLE;
        }
    }
    
}

//Desenha um frame do sprite do jogador com base em sua posição visual (baseada na tela)
void DrawPlayer(tPlayer *player, tMap *map){

        DrawRectangle(player->screenPos.x, player->screenPos.y, map->tile_size, map->tile_size, VIOLET);
    

}