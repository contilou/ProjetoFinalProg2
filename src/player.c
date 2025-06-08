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
        
        if (move_direction.x == 0 && move_direction.y == 0) {
            return;
        }

        Vector2 final_move = {0};

        // 2. Lógica de checagem com base na prioridade atual
        if (player->horizontal_priority) {
            // ---- PRIORIDADE HORIZONTAL ----
            // Tenta o movimento HORIZONTAL primeiro
            if (move_direction.x != 0 && map->matrix[player->matrixPos.row][player->matrixPos.column + (int)move_direction.x] == ' ') {
                final_move.x = move_direction.x;
            }
            // Se não deu, tenta o VERTICAL
            else if (move_direction.y != 0 && map->matrix[player->matrixPos.row + (int)move_direction.y][player->matrixPos.column] == ' ') {
                final_move.y = move_direction.y;
            }

        } else {
            // ---- PRIORIDADE VERTICAL ----
            // Tenta o movimento VERTICAL primeiro
            if (move_direction.y != 0 && map->matrix[player->matrixPos.row + (int)move_direction.y][player->matrixPos.column] == ' ') {
                final_move.y = move_direction.y;
            }
            // Se não deu, tenta o HORIZONTAL
            else if (move_direction.x != 0 && map->matrix[player->matrixPos.row][player->matrixPos.column + (int)move_direction.x] == ' ') {
                final_move.x = move_direction.x;
            }
        }

        // 3. Aplica o movimento final (nenhuma mudança aqui)
        if (final_move.x != 0 || final_move.y != 0) {
            player->matrixPos.row += final_move.y;
            player->matrixPos.column += final_move.x;
            player->state = MOVING;
            player->direction = final_move;
        }

        // 4. Inverte a prioridade para a PRÓXIMA chamada da função!
        player->horizontal_priority = !player->horizontal_priority;
    }
    //Se o jogador já estiver no estado MOVING,ou seja, a posição já matriz já tenha sido alterada), a posição do jogador na tela é alterada gradualmente em cada chamada da função
    //trazendo um efeito de suavização
    else if (player->state == MOVING){

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