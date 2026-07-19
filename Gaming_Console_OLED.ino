#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <EEPROM.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

byte selected = 0;
byte chooseGame = 10;
byte chooseGameGND = 11;

byte EndScore;

bool launchSnake;
bool launchPong;

bool DoublePong;
bool SinglePong;

bool chooseLock;

byte UP = 2;
byte DOWN = 3;
byte LEFT = 4;
byte LGND = 5;
byte RIGHT = 6;
byte RGND = 7;
byte buzz = 8;
byte buzzGND = 9;

int SnakeReady = false;

bool SnakeStartGameLock;

unsigned long lastMoveTime = 0;

byte x = 60;
byte y = 30;
int SnakeScore = 0;
int SnakeBest = 0;
unsigned long speed;
int Eaten;

bool easy = false;
bool medium = false;
bool hard = false;
bool insane = false;
bool snakeMenuLock;

const byte topBar = 9;

const byte mLen = 20;

byte snakeX[mLen];
byte snakeY[mLen];

int len = 3;

byte dX = 1;
byte dY = 0;

byte cell = 3;

byte foodX;
byte foodY;

bool dead = false;

bool inSnakeMenu;

bool directionChanged = false;

byte PadHits = 0;

byte PaddleSpeed = 2;

bool lastUp = HIGH;
bool ExtremePong = false;

bool InPong = false;
bool PongReady = false;

byte ScoreP1;
byte ScoreP2;

bool PADDLE1UP = false;
bool PADDLE1DOWN = false;
bool PADDLE2UP = false;
bool PADDLE2DOWN = false;

float paddle1Y = 20.0;
const byte paddle1X = 3;
float paddleH = 15.0;

float paddle2Y = 20.0;
const byte paddle2X = 122;

float ballDX;
float ballDY;

float ballX = 64.0 + random(-4, 5);
float ballY = 32.0 + random(-4, 5);

//Generate food randomly
//multiplied divided and multiplied by cell to stick on the 5x5 grid, so the snake can eat it
void makeFood() {
  int minY = ((topBar + cell - 1) / cell) * cell;
  int maxY = 63 - cell;

  foodX = random(0, (128 - cell) / cell) * cell;
  foodY = random(minY / cell, maxY / cell) * cell;
}

bool choosePressed() {

  static bool lastState = HIGH;

  bool current = digitalRead(chooseGame);

  bool pressed = (lastState == HIGH && current == LOW);

  lastState = current;

  return pressed;

}

void setup() {
  randomSeed(analogRead(A0));

  EEPROM.get(0, SnakeBest);
  if (SnakeBest < 0 || SnakeBest > 50000) SnakeBest = 0;
  
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(LGND, OUTPUT);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(RGND, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(buzzGND, OUTPUT);
  pinMode(chooseGameGND, OUTPUT);
  pinMode(chooseGame, INPUT_PULLUP);
  Serial.begin(9600);

  if (ExtremePong) {
    ballDX = 3.0;
    ballDY = 3.0;
  } else {
    ballDX = 2.0;
    ballDY = 2.0;
  }

  if (random(2) == 0) ballDX = -ballDX;
  if (random(2) == 0) ballDY = -ballDY;

  digitalWrite(LGND, LOW);
  digitalWrite(RGND, LOW);
  digitalWrite(buzzGND, LOW);
  digitalWrite(chooseGameGND, LOW);

  display.begin(0x3C, true);
  display.clearDisplay();

//Initial position of snake in ROUGHLY the middle of the OLED
  for (int i = 0; i < len; i++) {
    snakeX[i] = 60 - i;
    snakeY[i] = 30;
  }

  makeFood();
}

void startSnakeGame() {
  display.clearDisplay();

  SnakeStartGameLock = true;
  SnakeReady = false;
  inSnakeMenu = false;
  dead = false;
  len = 3;
  dX = 1;
  dY = 0;
  SnakeScore = 0;
  Eaten = 0;
  
  snakeX[0] = (60 / cell) * cell;
  snakeY[0] = (30 / cell) * cell;

  for (int i = 1; i < len; i++) {
    snakeX[i] = 60 - i * cell;
    snakeY[i] = 30;
  }
  makeFood();
}

void SnakeMenu() {

  if (choosePressed()) {
    launchSnake = false;
    inSnakeMenu = false;
    return;
  }

  if (dead == true) {
  easy = false;
  medium = false;
  hard = false;
  insane = false;

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  
  if (SnakeBest < SnakeScore) {
  display.setCursor(12, 15);
  display.print(F("NEW BEST!")); 

  display.setTextSize(1);
  display.setCursor(37, 40);
  display.print(F("Score: "));
  display.print(SnakeScore);

  display.setCursor(37, 50);
  display.print(F("Best: "));
  display.print(SnakeBest);
  display.display();

  display.display();
    
  tone(buzz, 1000, 500);
  delay(1000);
  tone(buzz, 1000, 500);
  delay(1000);
  tone(buzz, 1000, 500);
  delay(1000);

  }

  if (SnakeBest >= SnakeScore) {
  display.setCursor(13, 15);
  display.print(F("YOU DIED!")); 

  display.setTextSize(1);
  display.setCursor(37, 40);
  display.print(F("Score: "));
  display.print(SnakeScore);

  display.setCursor(37, 50);
  display.print(F("Best: "));
  display.print(SnakeBest);
  display.display();

  display.display();

  tone(buzz, 700, 500);
  delay(500);
  tone(buzz, 500, 500);
  delay(500);
  tone(buzz, 700, 500);
  delay(500);
  tone(buzz, 500, 500);
  delay(500);
  tone(buzz, 700, 500);
  delay(500);
  tone(buzz, 500, 500);
  delay(500);
  }

  if (SnakeBest < SnakeScore) {
    SnakeBest = SnakeScore;
    EEPROM.put(0, SnakeBest);
  }
  dead = false;
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(3, 1);
  display.print(F("SNAKE GAME"));
  
  display.setTextSize(1);
  display.setCursor(40, 20);
  display.print(F("UP - Easy"));

  display.setCursor(25, 28);
  display.print(F("LEFT - Medium"));

  display.setCursor(28, 36);
  display.print(F("RIGHT - Hard"));

  display.setCursor(25, 44);
  display.print(F("DOWN - INSANE"));


  display.setCursor(7, 56);
  display.print(F("Score: "));
  display.print(SnakeScore);

  display.setCursor(69, 56);
  display.print(F("Best: "));
  display.print(SnakeBest);
  display.display();
}

void SnakeGame() {
  // X limit = 127
  // Y limit = 63

  if (choosePressed()) {
    launchSnake = false;
    inSnakeMenu = false;
    SnakeStartGameLock = false;
    snakeMenuLock = false;
    return;
  }

  if (millis() - lastMoveTime < speed)
  return;

  bool anyPress = digitalRead(UP) == LOW || digitalRead(DOWN) == LOW || digitalRead(LEFT) == LOW || digitalRead(RIGHT) == LOW;

  if (inSnakeMenu && snakeMenuLock) {
    if (!anyPress) {
      snakeMenuLock = false;
    }
    return;
  }

  if (!inSnakeMenu && SnakeStartGameLock) {
    if (!anyPress) {
      SnakeStartGameLock = false;
    }
    return;
  }

  directionChanged = false;

  bool upPress = digitalRead(UP) == LOW;
  bool downPress = digitalRead(DOWN) == LOW;
  bool leftPress = digitalRead(LEFT) == LOW;
  bool rightPress = digitalRead(RIGHT) == LOW;

  if (inSnakeMenu && !snakeMenuLock) {
    SnakeMenu();
  if (upPress) {
      speed = 50;
      cell = 3;
      easy = true;
      startSnakeGame();
    }

    if(leftPress) {
      speed = 30;
      cell = 4;
      medium = true;
      startSnakeGame();
    }

    if(rightPress) {
      speed = 15;
      cell = 5;
      hard = true;
      startSnakeGame();
    }

      if(downPress) {
      speed = 5;
      cell = 6;
      insane = true;
      startSnakeGame();
    }
    return;
  }

//Direction switching which checks value of the joystick's analogRead to move.

//It checks if the opposite direction e.g. up/down, left/right, is not already enabled-
//-because if so, the snake will kill itself as it will collide with itself.

  if (upPress == true && dY != 1 && dX != 0 && !directionChanged) {dX = 0; dY = -1; tone(buzz, 600, 100); directionChanged = true;}
  if (downPress == true && dY != -1 && dX != 0 && !directionChanged) {dX = 0; dY = 1;tone(buzz, 600, 100); directionChanged = true;}

  if (leftPress == true && dX != 1 && dY != 0 && !directionChanged) {dX = -1; dY = 0;tone(buzz, 600, 100); directionChanged = true;}
  if (rightPress == true && dX != -1 && dY != 0 && !directionChanged) {dX = 1; dY = 0;tone(buzz, 600, 100); directionChanged = true;}

//body movement
  for (int i = len - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

//head movement
//multiplied by cell to ensure head locks onto the 5x5 grid
  snakeX[0] += dX * cell;
  snakeY[0] += dY * cell;

//self collision
for (int i = 1; i < len; i++) {
  if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
    tone(buzz, 400);
    delay(500);
    noTone(buzz);
    dead = true;
    inSnakeMenu = true;
    snakeMenuLock = true;
    return;

}
  }

//wall collision
if (snakeX[0] < 0 || snakeX[0] > 127 - cell || snakeY[0] < topBar || snakeY[0] > 63 - cell ) {
    tone(buzz, 400, 500);
    delay(750);
    noTone(buzz);
    dead = true;
    inSnakeMenu = true;
    snakeMenuLock = true;
  return;
}

//Interaction of food
if (snakeX[0] == foodX && snakeY[0] == foodY) {

  if (len < mLen) len++;
  Eaten += 1;

  if (easy) {
    SnakeScore += 2;
  }

  if (medium) {
    SnakeScore += 3;
  }

  if (hard) {
    SnakeScore += 4;
  }

  if (insane) {
    SnakeScore += 5;
  }

  makeFood();

  tone(buzz, 1000, 200);
}

display.clearDisplay();

//score
display.setTextSize(1);
display.setTextColor(SH110X_WHITE);
display.setCursor(2, 1);
display.print(F("Score: "));
display.print(SnakeScore);

//food eaten
display.setCursor(80, 1);
display.print(F("Food: "));
display.print(Eaten);

//border
display.drawRect(0, topBar, 127, 54, SH110X_WHITE);

//making food appear
if (foodX >= 0 && foodY >= 0) {
  display.fillTriangle(foodX + cell/2, (foodY), foodX, (foodY) + cell, foodX + cell, (foodY) + cell, SH110X_WHITE);
}

//making snake appear
for (int i = 0; i < len; i++) {
  display.fillRect(snakeX[i], (snakeY[i]), cell, cell, SH110X_WHITE);
}

//Countdown
if (!SnakeReady) {
  
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.print(F("READY?"));
  display.display();

  delay(1000);

  display.setCursor(87, 10);
  display.print(F("GO!"));
  display.display();

  delay(500);

  SnakeReady = true;
  
}

//BOTH replaced sixes with "cell" for ensuring that the grid is followed
lastMoveTime = millis();

display.display();

}

void StartPongGame() {

PongReady = false;

  if (ballX < 0) {
  display.setTextSize(2);
  display.setCursor(15, 32);
  display.print(F("P2 POINT!"));
  display.display();

  tone(buzz, 500, 250);
  delay(250);
  tone(buzz, 600, 250);
  delay(250);
  tone(buzz, 700, 250);
  delay(250);
  }

  if (ballX > 127) {
  display.setTextSize(2);
  display.setCursor(15, 32);
  display.print(F("P1 POINT!"));
  display.display();
  
  tone(buzz, 700, 250);
  delay(250);
  tone(buzz, 600, 250);
  delay(250);
  tone(buzz, 500, 250);
  delay(250);
  }

  display.clearDisplay();
  display.display();
  delay(100);

  if (ExtremePong) {
    ballDX = 3.0;
    ballDY = 3.0;
  } else {
    ballDX = 2.0;
    ballDY = 2.0;
  }

  ballX = 64 + random(-4, 5);
  ballY = 32 + random(-4, 5);

  if (random(2) == 0) ballDX = -ballDX;
  if (random(2) == 0) ballDY = -ballDY;

  PadHits = 0;

  paddle1Y = 20;
  paddle2Y = 20;

  PADDLE1UP = false;
  PADDLE1DOWN = false;
  PADDLE2UP = false;
  PADDLE2DOWN = false;

}

void PongMenu() {

bool choose = choosePressed();

if (choose) {
  launchPong = false;
  InPong = false;
  return;
}

ScoreP1 = 0;
ScoreP2 = 0;

display.clearDisplay();

display.setTextColor(SH110X_WHITE);

display.setTextSize(2);
display.setCursor(40, 1);
display.print(F("PONG"));

display.setTextSize(1);
display.setCursor(1, 32);
display.print(F("< 1Player  2Player >"));

bool UpNow = digitalRead(UP);

if (lastUp == HIGH && UpNow == LOW) {
ExtremePong = !ExtremePong;
}

lastUp = UpNow;

if (!ExtremePong) {
display.setCursor(2, 56);
display.print(F("EXTREME MODE: OFF (^)"));
ballDX = 2.0;
ballDY = 2.0;
}

if (ExtremePong) {
display.setCursor(6, 56);
display.print(F("EXTREME MODE: ON (^)"));
ballDX = 3.0;
ballDY = 3.0;
}

display.display();

}

void PongGame() {

display.clearDisplay();

if (choosePressed()) {
  InPong = false;
  PongReady = false;
  DoublePong = false;
  SinglePong = false;

  return;
}

if (ExtremePong) {
  paddleH = 10;
  EndScore = 10;
} else {paddleH = 15; EndScore = 7;}

//Drawing paddle
display.fillRect(paddle1X, paddle1Y, 3, paddleH, SH110X_WHITE);

display.fillRect(paddle2X, paddle2Y, 3, paddleH, SH110X_WHITE);

//Drawing ball
display.fillRect(ballX, ballY, 3, 3, SH110X_WHITE);

//Drawing Middle Line
display.drawLine(63, 0, 63, 9, SH110X_WHITE);

display.drawLine(63, 18, 63, 27, SH110X_WHITE);
display.drawLine(63, 36, 63, 45, SH110X_WHITE);

display.drawLine(63, 54, 63, 63, SH110X_WHITE);

//Drawing Borders
display.drawRect(0, 0, 128, 64, SH110X_WHITE);

if (!InPong) {
  PongMenu();
  if (digitalRead(RIGHT) == LOW) {
    InPong = true;
    PongReady = false;
    PadHits = 0;
    DoublePong = true;
    SinglePong = false;
    StartPongGame();
  }
  if (digitalRead(LEFT) == LOW) {
    InPong = true;
    PongReady = false;
    PadHits = 0;
    SinglePong = true;
    DoublePong = false;
    StartPongGame();
  }
  return;
}
//Readying players
if (!PongReady) {
  display.setTextSize(2);
  display.setCursor(17, 1);
  display.print(F("GET"));
  display.display();
  delay(300);

  display.setCursor(67, 50);
  display.print(F("READY"));
  display.display();
  delay(300);

  PongReady = true;
  return;
}

//Moving Paddles
if (digitalRead(UP) == LOW) {
  PADDLE2UP = true;
  PADDLE2DOWN = false;
}

if (digitalRead(DOWN) == LOW) {
  PADDLE2DOWN = true;
  PADDLE2UP = false;
}

if (PADDLE2UP) {
paddle2Y -= 2.5;
}

if (PADDLE2DOWN) {
  paddle2Y += 2.5;
}

if (DoublePong) {
if (digitalRead(LEFT) == LOW) {
  PADDLE1UP = true;
  PADDLE1DOWN = false;
}

if (digitalRead(RIGHT) == LOW) {
  PADDLE1DOWN = true;
  PADDLE1UP = false;
}

if (PADDLE1UP) {
  paddle1Y -= 2.5;
}

if (PADDLE1DOWN) {
  paddle1Y += 2.5;
}
}

if (SinglePong) {
  
  if (!ExtremePong) {
  if (ballY < paddle1Y + paddleH / 2)
  paddle1Y -= 5;

  if (ballY > paddle1Y + paddleH / 2)
  paddle1Y += 5;
  }

  if (ExtremePong) {
    if (ballY < paddle1Y + paddleH / 2)
    paddle1Y -= 10;

    if (ballY > paddle1Y + paddleH / 2)
    paddle1Y += 10;
  }
}

//Clamp paddle in screen
if (paddle1Y < 2) paddle1Y = 2;
if (paddle1Y > 62 - paddleH) paddle1Y = 62 - paddleH;

if (paddle2Y < 2) paddle2Y = 2;
if (paddle2Y > 62 - paddleH) paddle2Y = 62 - paddleH;

//Movement of ball

ballX += ballDX;
ballY += ballDY;

//bouncing from top/bottom
if (ballY <= 2 || ballY + 2 >= 62) {
  ballDY = -ballDY;

tone(buzz, 1000, 100);
}

//Ball/paddle bounce
if (ballX <= 3
&& ballX <= paddle1X + 3
&&  ballX + 2 >= paddle1X - fabs(ballDX)
&& ballY + 2 >= paddle1Y
&& ballY <= paddle1Y + paddleH) {
  ballDX = -ballDX;
  ballX = paddle1X + 3;
  tone(buzz, 1500, 100);

PadHits ++;

if (PadHits % 1 == 0  && fabs(ballDX) < 16 && fabs(ballDY) < 16) {

  if (ballDX > 0) ballDX += 0.25;
  else ballDX -= 0.25;

  if (ExtremePong) {
    if (ballDX > 0) ballDX += 0.5;
    else ballDX -= 0.5;
  }

  if (ballDY > 0) ballDY += 0.25;
  else ballDY -= 0.25;

  if (ExtremePong) {
    if (ballDY > 0) ballDY += 0.5;
    else ballDY -=0.5;
  }
}
}

if (ballX + 2 >= paddle2X
&& ballX <= paddle2X + 3 + fabs(ballDX)
&& ballY + 2 >= paddle2Y
&& ballY <= paddle2Y + paddleH) {
  ballDX = -ballDX;
  ballX = paddle2X - 3;
  tone(buzz, 1500, 100);

PadHits ++;

if (PadHits % 1 == 0  && fabs(ballDX) < 16 && fabs(ballDY) < 16) {

  if (ballDX > 0) ballDX += 0.25;
  else ballDX -= 0.25;

  if (ExtremePong) {
  if (ballDX > 0) ballDX += 0.5;
  else ballDX -= 0.5;
  }

  if (ballDY > 0) ballDY += 0.25;
  else ballDY -= 0.25;

  if (ExtremePong) {
   if (ballDY > 0) ballDY += 0.5;
  else ballDY -=0.5;
}
}
}

//Drawing Hits
display.setCursor(2, 2);
display.setTextSize(1);
display.print(PadHits);

//Point System
if (ballX < 0) {
  ScoreP2 += 1;
}

if (ballX > 127) {
  ScoreP1 += 1;
}

//Scores
display.setTextSize(1);
display.setTextColor(SH110X_WHITE);

if (ScoreP1 < 10) {
  display.setCursor(49, 3);
}

if (ScoreP1 >= 10 && ScoreP1 < 100) {
  display.setCursor(42, 3);
}

if (ScoreP1 >= 100 && ScoreP1 < 1000) {
  display.setCursor(35, 3);
}

display.print(ScoreP1);

display.setCursor(72, 3);
display.print(ScoreP2);

//Reset on missing
if (ballX < 0 || ballX > 127) {
  StartPongGame();
}

//Finishing Scene
if (ScoreP1 == EndScore) {
display.clearDisplay();

if (DoublePong) {
display.setTextSize(2);
display.setCursor(20, 1);
display.print(F("P1 WINS!"));
}

if (SinglePong) {
display.setTextSize(2);
display.setCursor(20, 1);
display.print(F("AI WINS!"));
}


display.setTextSize(1);
display.setCursor(42, 25);

if (DoublePong) display.print(F("P1"));
if (SinglePong) display.print(F("AI"));
display.setTextSize(2);
display.setCursor(35, 35);
display.print(ScoreP1);

display.setTextSize(1);
display.setCursor(75, 25);

if (DoublePong) display.print(F("P2"));
if (SinglePong) display.print(F("HUMAN"));
display.setTextSize(2);
display.setCursor(75, 35);
display.print(ScoreP2);

display.display();

delay(3000);

InPong = false;
}

if (ScoreP2 == EndScore) {
display.clearDisplay();

display.setTextSize(2);
display.setCursor(20, 1);

if (DoublePong) display.print(F("P2 WINS!"));
if (SinglePong) display.print(F("HUMAN WINS!"));

display.setTextSize(1);
display.setCursor(42, 25);
if (DoublePong) display.print(F("P2"));
if (SinglePong) display.print(F("HUMAN"));
display.setTextSize(2);
display.setCursor(35, 35);
display.print(ScoreP2);

display.setTextSize(1);
display.setCursor(75, 25);
if (DoublePong) display.print(F("P1"));
if (SinglePong) display.print(F("AI"));
display.setTextSize(2);
display.setCursor(75, 35);
display.print(ScoreP1);

display.display();

delay(3000);

InPong = false;
}

display.display();

}

void loop() {

//Pixel Game Menu Calcs:
//Width = 127 (total x) / 2 = 64      64 - 5 (for space between x in game menus) = 59 width
//Height = 63 (total y) / 2 = 32      32 - 7 (for space between y in game menus) = 25 height

display.clearDisplay();

bool leftPress = digitalRead(LEFT) == LOW;
bool rightPress = digitalRead(RIGHT) == LOW;

bool chosen = digitalRead(chooseGame);

if (chooseLock) {
  if (!chosen) {
    chooseLock = false;
  }
  return;
}

if (launchSnake) {
  SnakeGame();
  return;
}

if (launchPong) {
  PongGame();
  return;
}

//Snake Game
display.drawRect(5, 5, 53, 59, SH110X_WHITE);

display.fillRect(12, 12, 4, 10, SH110X_WHITE);
display.fillRect(16, 18, 15, 4, SH110X_WHITE);
display.fillTriangle(40, 22, 42, 20, 44, 22, SH110X_WHITE);
display.fillTriangle(47, 15, 49, 12, 51, 15, SH110X_WHITE);
display.setTextColor(SH110X_WHITE);
display.setTextSize(2);
display.setCursor(8, 40);
display.print(F("SNAK"));


//Snake
if (selected == 0) {
  display.drawRect(7, 7, 49, 55, SH110X_WHITE);
}

//Pong
display.drawRect(69, 5, 53, 59, SH110X_WHITE);

display.fillRect(75, 15, 3, 10, SH110X_WHITE);
display.fillRect(113, 9, 3, 10, SH110X_WHITE);
display.fillRect(94, 13, 3, 3, SH110X_WHITE);
display.setTextColor(SH110X_WHITE);
display.setTextSize(2);
display.setCursor(73, 40);
display.print(F("PONG"));


if (selected == 1) {
  display.drawRect(71, 7, 49, 55, SH110X_WHITE);
}

//Selection Border
if (rightPress && selected == 0) {
  selected = 1;
  tone(buzz, 200, 100);
}

if (leftPress && selected == 1) {
  selected = 0;
  tone(buzz, 200, 100);
}

if (selected == 0 && choosePressed()) {
  launchSnake = true;
  inSnakeMenu = true;
  tone(buzz, 400, 100);
  return;
  }

if (selected == 1 && choosePressed()) {
  launchPong = true;
  tone(buzz, 400, 100);
  return;
  }

display.display();

}
