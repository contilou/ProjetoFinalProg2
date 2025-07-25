#include <raylib.h>
#include <math.h>
#include "player.h"
#include "enemy.h"
#include "sounds.h"

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
void MovePlayer(tPlayer *player, tMap *map, AudioManager audio){

    //Sistema para sempre verificar e mover o jogador enquanto ele estiver invencível
    if (player->is_invincible) {
        player->invincibility_timer -= GetFrameTime();
        if (player->invincibility_timer <= 0) {
            player->is_invincible = false;
        }
    }
    
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

        Vector2 goal_direction = {0};

        // Realiza a checagem com base na lógica de prioridades dos eixos
        if (player->horizontal_priority) {
            // Para a prioridade horizontal, vai tentar o movimento horizontal primeiro
            if (move_direction.x != 0 && !isElementSolid(map->matrix[player->matrixPos.row][player->matrixPos.column + (int)move_direction.x])) {
                goal_direction.x = move_direction.x;
                player->horizontal_priority = false;

            }
            // caso contrário, tentara o vertical
            else if (move_direction.y != 0 && !isElementSolid(map->matrix[player->matrixPos.row + (int)move_direction.y][player->matrixPos.column])) {
                goal_direction.y = move_direction.y;
                player->horizontal_priority = true;
                
            }

        } else {
            // Para a prioridade vertical, vai tentar o movimento vertical primeiro
            if (move_direction.y != 0 && !isElementSolid(map->matrix[player->matrixPos.row + (int)move_direction.y][player->matrixPos.column])){
                goal_direction.y = move_direction.y;
                player->horizontal_priority = true;
            }
            // Caso contrário, tentara o horizontal
            else if (move_direction.x != 0 && !isElementSolid(map->matrix[player->matrixPos.row][player->matrixPos.column + (int)move_direction.x])) {
                goal_direction.x = move_direction.x;
                player->horizontal_priority = false;
            }
        }

        // Realiza o movimento com base na direção
        if (goal_direction.x != 0 || goal_direction.y != 0) {
            if(map->matrix[player->matrixPos.row][player->matrixPos.column] == 'J'){
                map->matrix[player->matrixPos.row][player->matrixPos.column] = ' ';
            }
            player->matrixPos.row += goal_direction.y;
            player->matrixPos.column += goal_direction.x;
            if(map->matrix[player->matrixPos.row][player->matrixPos.column] == 'C'){
                map->matrix[player->matrixPos.row][player->matrixPos.column] = 'J';
                PlaySound(audio.somChave);
                player->keys++;
                
            }
            player->state = MOVING;
            player->direction = goal_direction;
        }
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

bool isElementSolid(char elem){

    switch (elem) {
        case 'E':
        case 'C': 
        case ' ': 
            return false;
        default:
            return true;
    }

}
void ChangeScore(tPlayer *player, int score){
    player->score += score;
    player->score = (player->score < 0) ? 0 : player->score;
}

//Desenha um frame do sprite do jogador com base em sua posição visual (baseada na tela)
void DrawPlayer(tPlayer *player, tMap *map){

     // Se o jogador estiver invencível, ele pisca.                                                                 
        if (player->is_invincible) {
            // A função fmod(dividendo, divisor) retorna o resto da divisão.
            // Usamos para criar um efeito de piscar a cada 0.2 segundos.   
            if (fmod(player->invincibility_timer, 0.2f) > 0.1f) {
                DrawTexture(player->player_sprite, player->screenPos.x, player->screenPos.y, WHITE);
            }else{
                DrawTexture(player->player_sprite, player->screenPos.x, player->screenPos.y, RED);
            }
        } else {
            DrawTexture(player->player_sprite, player->screenPos.x, player->screenPos.y, WHITE);
        }

}

//Aplica dano ao jogador e torna ele invencível por um tempo.
void DamagePlayer(tPlayer *player, AudioManager audio){
    if (!player->is_invincible) {
        PlaySound(audio.somDano);
        player->lives--;
        player->is_invincible = true;
        player->invincibility_timer = 1.5f; // 1.5 segundos de invencibilidade
        ChangeScore(player, -100);
    }
}

bool DamageByEnemies(EnemyGroup *group, tPlayer *player){
    // Loop para verificar cada inimigo no grupo
    for (int i = 0; i < group->count; i++) {
        // Compara a posição do inimigo (usando 'group') com a do jogador (usando 'player')
        if (group->enemies[i].matrixPos.row == player->matrixPos.row && 
            group->enemies[i].matrixPos.column == player->matrixPos.column &&
            group->enemies[i].isAlive)
        {
            return true; // Retorna verdadeiro se encontrou colisão
        }
    }

    return false; // Retorna falso se o loop terminar sem colisões
}