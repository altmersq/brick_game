#include "fsm.h"
#include "tetris_backend.h"
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

static GameState current_state = STATE_START;
static double last_fall_time; 

double get_current_time_in_seconds() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void fsm_init() {
    current_state = STATE_START;
    last_fall_time = get_current_time_in_seconds();
}

void fsm_update(GameInfo_t *gameInfo, int current_piece[4][4], int *current_x, int *current_y) {

    if (gameInfo->pause) {
        return;
    }

    double current_time = get_current_time_in_seconds();

    switch (current_state) {
        case STATE_START:
            if (gameInfo->pause == 0) {
                current_state = STATE_SPAWN;
            }
            break;
        case STATE_SPAWN:
            spawn_new_piece();
            current_state = STATE_MOVE;
            break;
        case STATE_MOVE:
            if (current_time - last_fall_time >= 1.0 / gameInfo->speed) {
                current_state = STATE_SHIFT;
                last_fall_time = current_time;
            }
            break;
        case STATE_SHIFT:
            if (!move_piece_down(current_piece, current_x, current_y)) {
                current_state = STATE_COLLISION;
            } else {
                current_state = STATE_MOVE; 
            }
            break;
        case STATE_COLLISION:
            add_piece_to_field(gameInfo, current_piece, *current_x, *current_y);
            if (check_and_remove_full_lines(gameInfo)) {
                update_score(gameInfo);
                current_state = STATE_LINE_CLEAR;
            } else {
                if (check_game_over(gameInfo)) {
                    current_state = STATE_GAME_OVER;
                } else {
                    current_state = STATE_SPAWN;
                }
            }
            break;
        case STATE_LINE_CLEAR:
            current_state = STATE_SPAWN;
            break;
        case STATE_GAME_OVER:
                // save_high_score("high_score.txt", gameInfo->high_score);
                // mvprintw(10, 80, "saved high score");
                // getch();
            break;
    }
}

void fsm_handle_input(UserAction_t action, GameInfo_t *gameInfo, int current_piece[4][4], int *current_x, int *current_y) {
    if (action == Pause) {
        gameInfo->pause = !gameInfo->pause;
    }

    if (current_state == STATE_MOVE && !gameInfo->pause) {
        switch (action) {
            case Left:
                if (can_move(current_piece, *current_x, *current_y, -1, 0)) {
                    (*current_x)--;
                }
                break;
            case Right:
                if (can_move(current_piece, *current_x, *current_y, 1, 0)) {
                    (*current_x)++;
                }
                break;
            case Down:
                while (can_move(current_piece, *current_x, *current_y, 0, 1)) {
                    (*current_y)++;
                }
                current_state = STATE_COLLISION;
                return;
            case Action:
                rotate_piece(current_piece, *current_x, *current_y);
                break;
            case Start:
                fsm_init();
                break;
            default:
                break;
        }
        
        current_state = STATE_SHIFT;
    }
}

bool check_game_over(GameInfo_t *gameInfo) {
    for (int j = 0; j < 10; j++) {
        if (gameInfo->field[1][j] != 0) {
            current_state = STATE_GAME_OVER;
            return true;
        }
    }
    return false;
}

GameState get_current_state() {
    return current_state;
}