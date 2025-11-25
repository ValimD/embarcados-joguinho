#include "DinoGame.h"

// Constructor: Initializes the internal reference 'lcd' and 'buttonPin'
DinoGame::DinoGame(LiquidCrystal& lcdRef, int btnPin) : lcd(lcdRef), buttonPin(btnPin) {}

void DinoGame::setup() {
    // Create custom characters
    lcd.createChar(0, playerChar);
    lcd.createChar(1, obstacleChar);

    // Setup the button pin (only needs to be done once, but harmless here)
    pinMode(buttonPin, INPUT);

    // Display introductory message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" LCD DINO GAME ");
    lcd.setCursor(0, 1);
    lcd.print(" Press to jump ");
    delay(1000);

    resetGame();
}

// Resets game state (position, timing)
void DinoGame::resetGame() {
    currentStatus = PLAYING;
    jumping = false;
    obstacleX = 15;
    jumpStart = 0;
    lastMove = millis();
    lcd.clear();
}

// Handles button press and jump timing
void DinoGame::handleJump() {
    // Check for jump initiation
    // NOTE: This digitalRead must not cause the universal exit in the main loop
    // because the main loop checks for the button press *before* calling this function.
    if (digitalRead(buttonPin) == HIGH && !jumping) {
        jumping = true;
        jumpStart = millis();
    }

    // End jump after jumpDuration
    if (jumping && millis() - jumpStart > jumpDuration) {
        jumping = false;
    }
}

// Handles obstacle movement and respawning
void DinoGame::moveObstacle() {
    if (millis() - lastMove > moveInterval) {
        obstacleX--;
        lastMove = millis();

        if (obstacleX < 0) {
            obstacleX = 15; // respawn at the right edge
        }
    }
}

// Checks for collision between player (at X=1) and obstacle (at X=obstacleX)
bool DinoGame::checkCollision() {
    // Collision occurs if the player is on the ground (not jumping) AND the obstacle is at column 1
    return (!jumping && obstacleX == 1);
}

// Clears the screen and redraws player and obstacle
void DinoGame::draw() {
    lcd.clear();

    // Draw player: Row 0 is jump, Row 1 is ground
    int playerRow = jumping ? 0 : 1;
    lcd.setCursor(1, playerRow);
    lcd.write(byte(0));

    // Draw obstacle (always on row 1 for this game)
    if (obstacleX >= 0 && obstacleX < 16) {
      lcd.setCursor(obstacleX, 1);
      lcd.write(byte(1));
    }
}

// Draws the game over message and waits for restart button press (non-blocking)
void DinoGame::drawGameOver() {
    static unsigned long lastDisplayed = 0;
    // We only clear/redraw every half second to reduce flicker and effort
    if (millis() - lastDisplayed > 500) {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("GAME OVER!");
        lcd.setCursor(0, 1);
        lcd.print("Jump to restart");
        lastDisplayed = millis();
    }
}

// The main function for the game logic, called repeatedly by the Arduino loop()
void DinoGame::run() {
    if (currentStatus == PLAYING) {
        handleJump();
        moveObstacle();

        if (checkCollision()) {
            // Collision! Transition to GAME_OVER state
            currentStatus = GAME_OVER;
            // Immediate redraw to show Game Over screen
            drawGameOver();
        } else {
            draw();
        }
    } else if (currentStatus == GAME_OVER) {
        drawGameOver(); // Keep refreshing the game over screen

        // Check for restart via the dedicated jump button (non-blocking check)
        if (digitalRead(buttonPin) == HIGH) {
            resetGame(); // Transitions currentStatus back to PLAYING
        }
    }
}
