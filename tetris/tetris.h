#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>

#define FIELD_H 20
#define FIELD_W 10

#define ENTER_KEY 10

typedef enum {
    Start,
    Pause,
    Terminate,
    Left,
    Right,
    Up,
    Down,
    Action
} UserAction_t; 

typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;


// void userInput(UserAction_t action, bool hold);
// GameInfo_t updateCurrentState();


#endif // TETRIS_H