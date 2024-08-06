#include "fsm.h"
#include <stdlib.h>
#include "tetris_backend.h"
#include <time.h>
#include <sys/time.h>

static GameInfo_t gameInfo;
static int current_piece[4][4];
static int current_x;
static int current_y;

static int next_piece[4][4];

int **create_matrix(int H, int W) {
    int **matrix = (int **)malloc(H * sizeof(int *));
    for (int i = 0; i < H; i++) {
        matrix[i] = (int *)malloc(W * sizeof(int));
        for (int j = 0; j < W; j++) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

void generate_piece(int piece[4][4]) {
    static const int pieces[7][4][4] = {
        // I
        {{0, 0, 0, 0},
         {1, 1, 1, 1},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        // O
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}},
        // T
        {{0, 0, 0, 0},
         {0, 1, 0, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0}},
        // S
        {{0, 0, 0, 0},
         {0, 1, 1, 0},
         {1, 1, 0, 0},
         {0, 0, 0, 0}},
        // Z
        {{0, 0, 0, 0},
         {1, 1, 0, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0}},
        // J
        {{0, 0, 0, 0},
         {1, 0, 0, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0}},
        // L
        {{0, 0, 0, 0},
         {0, 0, 1, 0},
         {1, 1, 1, 0},
         {0, 0, 0, 0}}
    };

    int piece_idx = rand() % 7;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            piece[i][j] = pieces[piece_idx][i][j];
        }
    }
}

void spawn_new_piece() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            current_piece[i][j] = next_piece[i][j];
        }
    }
    generate_piece(next_piece);

    current_x = 3;
    current_y = 0;
}

void initGame() {
    srand(time(NULL));

    gameInfo.field = create_matrix(20, 10);
    gameInfo.next = create_matrix(4, 4);

    gameInfo.score = 0;
    gameInfo.high_score = load_high_score("high_score.txt");
    gameInfo.level = 1;
    gameInfo.speed = 1;
    gameInfo.pause = 0;

    generate_piece(current_piece);
    generate_piece(next_piece);
    spawn_new_piece();
    fsm_init();
}

void userInput(UserAction_t action, bool hold) {
    fsm_handle_input(action, &gameInfo, current_piece, &current_x, &current_y);
}

int can_move(int piece[4][4], int x, int y, int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece[i][j]) {
                int new_x = x + j + dx;
                int new_y = y + i + dy;
                if (new_x < 0 || new_x >= 10 || new_y < 0 || new_y >= 20 || gameInfo.field[new_y][new_x]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void rotate_piece(int piece[4][4], int x, int y) {
    int temp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = piece[3 - j][i];
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int new_x = x + j;
            int new_y = y + i;
            if (temp[i][j] && (new_x < 0 || new_x >= 10 || new_y < 0 || new_y >= 20 || gameInfo.field[new_y][new_x])) {
                return;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            piece[i][j] = temp[i][j];
        }
    }
}

void add_piece_to_field(GameInfo_t *gameInfo, int piece[4][4], int x, int y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece[i][j] == 1) {
                gameInfo->field[y + i][x + j] = 1;
            }
        }
    }
}

bool check_and_remove_full_lines(GameInfo_t *gameInfo) {
    bool line_cleared = false;
    for (int i = 0; i < FIELD_H; i++) {
        bool full = true;
        for (int j = 0; j < FIELD_W; j++) {
            if (gameInfo->field[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int k = i; k > 0; k--) {
                for (int l = 0; l < 10; l++) {
                    gameInfo->field[k][l] = gameInfo->field[k - 1][l];
                }
            }
            for (int l = 0; l < 10; l++) {
                gameInfo->field[0][l] = 0;
            }
            gameInfo->score += 100;
            line_cleared = true;
        }
    }
    return line_cleared;
}

void update_score(GameInfo_t *gameInfo) {
    int lines_cleared = 0;
    for (int i = 0; i < 20; i++) {
        bool full = true;
        for (int j = 0; j < 10; j++) {
            if (gameInfo->field[i][j] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            lines_cleared++;
            for (int k = i; k > 0; k--) {
                for (int l = 0; l < 10; l++) {
                    gameInfo->field[k][l] = gameInfo->field[k - 1][l];
                }
            }
            for (int l = 0; l < 10; l++) {
                gameInfo->field[0][l] = 0;
            }
        }
    }

    switch (lines_cleared) {
        case 1:
            gameInfo->score += 100;
            break;
        case 2:
            gameInfo->score += 300;
            break;
        case 3:
            gameInfo->score += 700;
            break;
        case 4:
            gameInfo->score += 1500;
            break;
    }

    if (gameInfo->score > gameInfo->high_score) {
        gameInfo->high_score = gameInfo->score;
    }

    gameInfo->level = gameInfo->score / 600 + 1;
    if (gameInfo->level > 10) {
        gameInfo->level = 10;
    }
    gameInfo->speed = gameInfo->level;
}

int move_piece_down(int piece[4][4], int *x, int *y) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece[i][j] == 1) {
                int new_y = *y + i + 1;
                if (new_y >= 20 || gameInfo.field[new_y][*x + j] == 1) {
                    return 0;
                }
            }
        }
    }
    (*y)++;
    return 1;
}

GameInfo_t updateCurrentState() {
    fsm_update(&gameInfo, current_piece, &current_x, &current_y);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gameInfo.next[i][j] = next_piece[i][j];
        }
    }
    return gameInfo;
}

int load_high_score(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        file = fopen(filename, "w");
        if (file == NULL) {
            perror("Ошибка создания файла для рекорда");
            return 0;
        }
        fprintf(file, "0");
        fclose(file);
        return 0;
    }

    int high_score = 0;
    fscanf(file, "%d", &high_score);
    fclose(file);
    return high_score;
}

void save_high_score(const char *filename, int high_score) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка открытия файла для записи рекорда");
        return;
    }

    fprintf(file, "%d", high_score);
    fclose(file);
}

int (*get_current_piece())[4] {
    return current_piece;
}

int get_current_x() {
    return current_x;
}

int get_current_y() {
    return current_y;
}