Hello, this is my arduino project I have been working on as my first rpject for hack club macondo.

It is a 2-game mini gaming console I have made on an OLED screen with arduino.

Equipment used:
-Jumper Wires
-V shaped Wires
-Arduino
-OLED Screen "SH1106"
-5 Push Buttons
-Passive Buzzer
-Large Breadboard

Features of the project:
-Snake Game
-EEPROM score saving feautres (Snake)
-Two/One player Pong
-Menu Interfaces
-Buzzer noises sounding on certain actions (press buttons, getting points, etc.)

The Pong_OLED and Snake_Game_OLED files are testing files which I have used to make each of the named games. In the end, i have combined the two together in the Gaming_Console_OLED
file, which additionally includes the game selection logic and menu code too.

Although most of the work has been done and dusted in this project, the only problem which still remains is the fact that the button to exit the pong menu only works intermittently.
I speculate that this is due to how the "choosePressed()" variable is used in various other parts of my code too, including this one, which can collide with eachother and cause
problems when the program is running. This though, I am not certain about, since I am still a beginenr to arduino and coding.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Here is how to setup the Physical components:

1. Connect the OLED display to the breadboard, around the centre

2. Afterwards, connect the OLED to the Arduino via the breadboard, via jumper wires-
  VCC to a 5V pin
  GND to a GND pin
  SCL to SCL pin
  SDA to SDA pin

3. Connect the buzzer to the breadboard, position on the right of the OLED on the breadboard

4. Via the breadboard and jumper wires, connect the positive side of the buzzer to pin 8 of the arduino, as well as the negative side of the buzzer to pin 9

5. For the UP push button: Position it on the top right of the breadboard

6. After doing so, connect the UP push button via jumper wires and U-shaped wires (for neatness) to pin 2 of the arduino, and also connecting the negative side of the button to a GND
Arduino

7. For the DOWN push button: Position it on the bottom right of the breadboard, under UP

8. After, connect the DOWN push button via jumper and u-shaped wires to pin 3 of the arduino, connecting the negative side to another Arduino GND too

9. For the LEFT Push button: Poistion it on the left side of the arduino, but in the centre

10. After, connect the LEFT Push Button to the arduino via jumper and u shaped wires, connecting positive to pin 4 and negative to pin 5

11. For the RIGHT Push button, position it on the left side of the arduino, on the centre, but to the right of the left button, for a gaming-console-like layout

12. After, connect the RIGHT push button to the arduino via jumper and u shaped wires, connecting the positive to pin 6 and negative to pin 7

13. For the Exiting/Enter game button: Position it on the bottom left of the OLED screen

14. Connect that button to the arduino via jumper and u-shaped wires, so that the positive side is connected to pin 10, and the negative connects to pin 11

15. Afterwards, get your Arduino cable and plug it into your device

16. Once connected, upload the code to it which is inside the Gaming_Console_OLED file

17. BOOM! ur done.
