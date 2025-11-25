#ifndef REACTIONGAME_H
#define REACTIONGAME_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class ReactionGame {
public:
    enum GameState {
        WAITING,
        COUNTDOWN,
        GO,
        FINISHED
    };

private:
    LiquidCrystal& lcd;

    // Pins for the two players
    const int player1Pin;
    const int player2Pin;

    // Game State
    GameState currentState = WAITING;
    unsigned long lastTime = 0;
    unsigned long startTime = 0;
    long goDelayMs = 0; // Random delay before GO

    // Result tracking
    int winner = 0; // 1 for P1, 2 for P2, 0 for Tie/Waiting
    unsigned long reactionTime = 0;


    // Custom characters (optional: simple indicators)
    byte p1Char[8] = { B10000, B11000, B10100, B10000, B10000, B10000, B10000, B10000 };
    byte p2Char[8] = { B00001, B00011, B00101, B00001, B00001, B00001, B00001, B00001 };

    // Private helper methods
    void resetGame();
    void stateWaiting();
    void stateCountdown();
    void stateGo();
    void stateFinished();
    void drawInstructions();

public:
    // Constructor now takes the two player pins
    ReactionGame(LiquidCrystal& lcdRef, int p1Pin, int p2Pin);

    void setup();
    void run();
    bool isGameComplete(); // Check if game is finished and ready to return to menu
};

#endif // REACTIONGAME_H
