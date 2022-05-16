#ifndef __GAME__H
#define __GAME__H


int getRandomValue();
int * getMatchSequence();
int getSequenceSize();
void resetSequence();
int incMatchSequence();
int playSequence();
int compareSequence();
int titleScreen();
enum GameState getGameState();
void nextGameState(enum GameState state);
void setGameState(enum GameState state);
enum GameState getGameState();
int startGame();
int gameOver();
enum GraphicsMode getGraphicsMode(); 
void setGraphicsMode(int16_t mode);

enum GraphicsMode {
    LCD_SCREEN,
    LED_MATRIX
};



enum GameState {
    TITLE_SCREEN,
    START_GAME,
    PLAY_SEQUENCE,
    COMPARE_SEQUENCE,
    INCREMENT_SEQUENCE_SIZE,
    GAME_OVER,
    NULL_STATE
};

typedef int(*game_function)();
typedef struct sGameStateMap
{
    enum GameState mState;
    game_function mFunction;
} sGameStateMap;


#endif