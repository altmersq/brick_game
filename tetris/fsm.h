#ifndef FSM_H
#define FSM_H

#include "tetris.h"


typedef enum {
    STATE_START,
    STATE_SPAWN,
    STATE_MOVE,
    STATE_SHIFT,
    STATE_COLLISION,
    STATE_LINE_CLEAR,
    STATE_GAME_OVER
} GameState;


void fsm_init();
void fsm_update(GameInfo_t *gameInfo, int current_piece[4][4], int *current_x, int *current_y);
void fsm_handle_input(UserAction_t action, GameInfo_t *gameInfo, int current_piece[4][4], int *current_x, int *current_y);
bool check_game_over(GameInfo_t *gameInfo);

GameState get_current_state();

#endif
