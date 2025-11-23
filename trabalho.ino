#include <LiquidCrystal.h>

// LCD pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Button pin
const int button = 6;

// Player state
bool jumping = false;
unsigned long jumpStart = 0;
const int jumpDuration = 1000; // ms

// Obstacle
int obstacleX = 15; // start at rightmost column
unsigned long lastMove = 0;
const int moveInterval = 200; // lower = faster

// Custom player char (a filled block)
byte playerChar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

// Obstacle char (a tall block)
byte obstacleChar[8] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};

void setup() {
  lcd.begin(16, 2);

  lcd.createChar(0, playerChar);
  lcd.createChar(1, obstacleChar);

  pinMode(button, INPUT);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" LCD DINO GAME ");
  lcd.setCursor(0, 1);
  lcd.print(" Press to jump ");
  delay(2000);

  lcd.clear();
}

void resetGame() {
  jumping = false;
  obstacleX = 15;
  jumpStart = 0;
  lastMove = millis();
  lcd.clear();
}

void loop() {

  // --- Player Jump Control ---
  if (digitalRead(button) == HIGH && !jumping) {
    jumping = true;
    jumpStart = millis();
  }

  // End jump
  if (jumping && millis() - jumpStart > jumpDuration) {
    jumping = false;
  }

  // --- Move Obstacle ---
  if (millis() - lastMove > moveInterval) {
    obstacleX--;
    lastMove = millis();

    if (obstacleX < 0) {
      obstacleX = 15; // respawn
    }
  }

  // --- Draw everything ---
  lcd.clear();

  // Draw player
  int playerRow = jumping ? 0 : 1;
  lcd.setCursor(1, playerRow);
  lcd.write(byte(0));

  // Draw obstacle
  lcd.setCursor(obstacleX, 1);
  lcd.write(byte(1));

  // --- Collision detection ---
  if (!jumping && obstacleX == 1) {
    // Collision!
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("GAME OVER!");
    lcd.setCursor(0, 1);
    lcd.print("Press button...");

    // wait for restart
    while (digitalRead(button) == LOW) {}
    delay(300);
    resetGame();
  }

  delay(30);
}
