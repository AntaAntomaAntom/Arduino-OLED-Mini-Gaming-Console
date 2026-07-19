#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <EEPROM.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

byte UP = 2;
byte DOWN = 3;
byte LEFT = 4;
byte LGND = 5;
byte RIGHT = 6;
byte RGND = 7;
byte buzz = 8;
byte buzzGND = 9;

byte x = 60;
byte y = 30;
int Score = 0;
int Best = 0;
byte speed;
int Eaten;

bool easy = false;
bool medium = false;
bool hard = false;
bool insane = false;

const byte topBar = 9;

const byte mLen = 30;

byte snakeX[mLen];
byte snakeY[mLen];

int len = 3;

byte dX = 1;
byte dY = 0;

byte cell = 3;

byte foodX;
byte foodY;

bool dead = false;

bool inMenu = true;

bool directionChanged = false;

//Generate food randomly
//multiplied divided and multiplied by cell to stick on the 5x5 grid, so the snake can eat it
void makeFood() {
  int minY = ((topBar + cell - 1) / cell) * cell;
  int maxY = 63 - cell;

  foodX = random(0, (128 - cell) / cell) * cell;
  foodY = random(minY / cell, maxY / cell) * cell;
}

void setup() {
  EEPROM.get(0, Best);
  if (Best < 0 || Best > 10000) Best = 0;
  
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(LGND, OUTPUT);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(RGND, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(buzzGND, OUTPUT);
  Serial.begin(9600);

  display.begin(0x3C, true);
  display.clearDisplay();

//Initial position of snake in ROUGHLY the middle of the OLED
  for (int i = 0; i < len; i++) {
    snakeX[i] = 60 - i;
    snakeY[i] = 30;
  }

  makeFood();
}

void startGame() {
  display.clearDisplay();
  inMenu = false;
  dead = false;
  len = 3;
  dX = 1;
  dY = 0;
  Score = 0;
  Eaten = 0;
  
  snakeX[0] = (60 / cell) * cell;
  snakeY[0] = (30 / cell) * cell;

  for (int i = 1; i < len; i++) {
    snakeX[i] = 60 - i * cell;
    snakeY[i] = 30;
  }
  makeFood();
}

void Menu() {

  if (dead == true) {
  easy = false;
  medium = false;
  hard = false;
  insane = false;

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  
  if (Best < Score) {
  display.setCursor(12, 15);
  display.print("NEW BEST!"); 

  display.setTextSize(1);
  display.setCursor(37, 40);
  display.print("Score: ");
  display.print(Score);

  display.setCursor(37, 50);
  display.print("Best: ");
  display.print(Best);
  display.display();

  display.display();
    
  tone(buzz, 1000, 500);
  delay(1000);
  tone(buzz, 1000, 500);
  delay(1000);
  tone(buzz, 1000, 500);
  delay(1000);

  }

  if (Best >= Score) {
  display.setCursor(13, 15);
  display.print("YOU DIED!"); 

  display.setTextSize(1);
  display.setCursor(37, 40);
  display.print("Score: ");
  display.print(Score);

  display.setCursor(37, 50);
  display.print("Best: ");
  display.print(Best);
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

  if (Best < Score) {
    Best = Score;
    EEPROM.put(0, Best);
  }
  dead = false;
  }
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(3, 1);
  display.print("SNAKE GAME");
  
  display.setTextSize(1);
  display.setCursor(40, 20);
  display.print("UP - Easy");

  display.setCursor(25, 28);
  display.print("LEFT - Medium");

  display.setCursor(28, 36);
  display.print("RIGHT - Hard");

  display.setCursor(25, 44);
  display.print("DOWN - INSANE");


  display.setCursor(7, 56);
  display.print("Score: ");
  display.print(Score);

  display.setCursor(75, 56);
  display.print("Best: ");
  display.print(Best);
  display.display();
}

void loop() {
  // X limit = 127
  // Y limit = 63

  directionChanged = false;

  digitalWrite(LGND, LOW);
  digitalWrite(RGND, LOW);
  digitalWrite(buzzGND, LOW);

  bool upPress = digitalRead(UP) == LOW;
  bool downPress = digitalRead(DOWN) == LOW;
  bool leftPress = digitalRead(LEFT) == LOW;
  bool rightPress = digitalRead(RIGHT) == LOW;

  if (inMenu) {
    Menu();
  if (upPress) {
      speed = 50;
      cell = 3;
      easy = true;
      startGame();
    }

    if(leftPress) {
      speed = 25;
      cell = 4;
      medium = true;
      startGame();
    }

    if(rightPress) {
      speed = 15;
      cell = 5;
      hard = true;
      startGame();
    }

      if(downPress) {
      speed = 5;
      cell = 6;
      insane = true;
      startGame();
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
    dead = true;
    inMenu = true;
    return;

}
  }

//wall collision
if (snakeX[0] < 0 || snakeX[0] > 127 - cell || snakeY[0] < topBar || snakeY[0] > 63 - cell ) {
    dead = true;
    inMenu = true;
  return;
}

//Interaction of food
if (snakeX[0] == foodX && snakeY[0] == foodY) {

  if (len < mLen) len++;
  Eaten += 1;

  if (easy) {
    Score += 2;
  }

  if (medium) {
    Score += 3;
  }

  if (hard) {
    Score += 4;
  }

  if (insane) {
    Score += 5;
  }

  makeFood();

  tone(buzz, 1000, 200);
}

display.clearDisplay();

//score
display.setTextSize(1);
display.setTextColor(SH110X_WHITE);
display.setCursor(2, 1);
display.print("Score: ");
display.print(Score);

//food eaten
display.setCursor(80, 1);
display.print("Food: ");
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
//BOTH replaced sixes with "cell" for ensuring that the grid is followed
display.display();
delay(speed);

}
