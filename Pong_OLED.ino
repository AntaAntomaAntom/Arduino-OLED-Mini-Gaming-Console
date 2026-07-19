#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

Adafruit_SH1106G display(128, 64, &Wire, -1);

const int UP1 = 2;
const int DOWN1 = 3;

byte UP = 2;
byte DOWN = 3;
byte LEFT = 4;
byte LGND = 5;
byte RIGHT = 6;
byte RGND = 7;
byte buzz = 8;
byte buzzGND = 9;

bool InPong = false;
bool PongReady = false;

int ScoreP1;
int ScoreP2;

bool PADDLE1UP = false;
bool PADDLE1DOWN = false;
bool PADDLE2UP = false;
bool PADDLE2DOWN = false;

int paddle1Y = 20;
const int paddle1X = 0;
const int paddleH = 15;

int paddle2Y = 20;
const int paddle2X = 124;

int ballX = 64;
int ballY = 32;

int BallXspeed = 3;
int BallYspeed = 3;

int ballDX = BallXspeed;
int ballDY = BallYspeed;

void setup() {
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(LGND, OUTPUT);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(RGND, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(buzzGND, OUTPUT);

  digitalWrite(LGND, LOW);
  digitalWrite(RGND, LOW);
  digitalWrite(buzzGND, LOW);

  display.begin(0x3C, true);
  display.clearDisplay();

}

void PongMenu() {

ScoreP1 = 0;
ScoreP2 = 0;

display.clearDisplay();

display.setTextSize(2);
display.setCursor(30, 1);
display.print("PONG");

display.setTextSize(1);
display.setCursor(30, 25);
display.print("VS AI - UP");
display.setCursor(20, 34);
display.print("VS Human - DOWN");

display.display();

}

void loop() {

display.clearDisplay();

if (InPong == false) {
  PongMenu();
  return();
}

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

//Readying players
if (!PongReady) {
  display.setTextSize(2);
  display.setCursor(17, 1);
  display.print("GET");
  display.display();
  delay(300);

  display.setCursor(67, 50);
  display.print("READY");
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
paddle2Y -= 2;
}

if (PADDLE2DOWN) {
  paddle2Y += 2;
}

if (digitalRead(LEFT) == LOW) {
  PADDLE1UP = true;
  PADDLE1DOWN = false;
}

if (digitalRead(RIGHT) == LOW) {
  PADDLE1DOWN = true;
  PADDLE1UP = false;
}

if (PADDLE1UP) {
  paddle1Y -= 2;
}

if (PADDLE1DOWN) {
  paddle1Y += 2;
}

//Clamp paddle in screen
if (paddle1Y < 0) paddle1Y = 0;
if (paddle1Y > 64 - paddleH) paddle1Y = 64 - paddleH;

if (paddle2Y < 0) paddle2Y = 0;
if (paddle2Y > 64 - paddleH) paddle2Y = 64 - paddleH;

//Movement of ball
ballX += ballDX;
ballY += ballDY;

//bouncing from top/bottom
if (ballY <= 0 || ballY >= 63) {
  ballDY = -ballDY;

tone(buzz, 1000, 100);
}

//Ball/paddle bounce
if (ballX <= 3
&& ballX <= paddle1X + 3
&&  ballX + 2 >= paddle1X
&& ballY + 2 >= paddle1Y
&& ballY <= paddle1Y + paddleH) {
  ballDX = -ballDX;
  ballX = paddle1X + 3;
  tone(buzz, 1500, 100);
}

if (ballX + 2 >= paddle2X
&& ballX <= paddle2X + 3
&& ballY + 2 >= paddle2Y
&& ballY <= paddle2Y + paddleH) {
  ballDX = -ballDX;
  ballX = paddle2X - 3;
  tone(buzz, 1500, 100);
}


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
  
  PongReady = false;

  if (ballX < 0) {
  display.setTextSize(2);
  display.setCursor(15, 32);
  display.print("P2 POINT!");
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
  display.print("P1 POINT!");
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

  ballX = 64;
  ballY = 32;
  ballDX = BallXspeed;
  ballDY = BallYspeed;

  paddle1Y = 20;
  paddle2Y = 20;

  PADDLE1UP = false;
  PADDLE1DOWN = false;
  PADDLE2UP = false;
  PADDLE2DOWN = false;
}

//Finishing Scene
if (ScoreP1 == 10) {
display.clearDisplay();

display.setTextSize(2);
display.setCursor(20, 1);
display.print("P1 WINS!");

display.setTextSize(1);
display.setCursor(42, 25);
display.print("P1");
display.setTextSize(2);
display.setCursor(35, 35);
display.print(ScoreP1);

display.setTextSize(1);
display.setCursor(75, 25);
display.print("P2");
display.setTextSize(2);
display.setCursor(75, 35);
display.print(ScoreP2);

display.display();

delay(3000);

PongMenu();
}

if (ScoreP2 == 10) {
display.clearDisplay();

display.setTextSize(2);
display.setCursor(20, 1);
display.print("P2 WINS!");

display.setTextSize(1);
display.setCursor(42, 25);
display.print("P2");
display.setTextSize(2);
display.setCursor(35, 35);
display.print(ScoreP2);

display.setTextSize(1);
display.setCursor(75, 25);
display.print("P1");
display.setTextSize(2);
display.setCursor(75, 35);
display.print(ScoreP1);

display.display();

delay(3000);

InPong = false;
}

display.display();

}
