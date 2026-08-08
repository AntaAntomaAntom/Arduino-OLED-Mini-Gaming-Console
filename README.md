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

<img width="2016" height="1512" alt="image" src="https://github.com/user-attachments/assets/40b1472c-1f4a-42d7-8116-f10470127602" />

Features of the project:
-Snake Game
-EEPROM score saving feautres (Snake)
-Two/One player Pong
-Menu Interfaces
-Buzzer noises sounding on certain actions (press buttons, getting points, etc.)

<img width="842" height="362" alt="image" src="https://github.com/user-attachments/assets/c0005074-f23b-49bd-ad18-b7047f6b3342" />
<img width="859" height="347" alt="image" src="https://github.com/user-attachments/assets/d975af61-b493-47fd-9a9a-aa8141044eb8" />
<img width="923" height="382" alt="image" src="https://github.com/user-attachments/assets/8d1b16eb-e789-4269-855a-2c3ad1ab1358" />

The Pong_OLED and Snake_Game_OLED files are testing files which I have used to make each of the named games. In the end, i have combined the two together in the Gaming_Console_OLED
file, which additionally includes the game selection logic and menu code too.

Although most of the work has been done and dusted in this project, the only problem which still remains is the fact that the button to exit the pong menu only works intermittently.
I speculate that this is due to how the "choosePressed()" variable is used in various other parts of my code too, including this one, which can collide with eachother and cause
problems when the program is running. This though, I am not certain about, since I am still a beginenr to arduino and coding. Additionally, the collision in the pong game is kinda weird, when the ball is too fast, it can sometimes clip through the paddles and make a player score a point.

Please refer to the symbol and physical wiring schematics which I have uploaded to this repo. Note that I have used a smaller different sized OLED in the physical wiring diagram since I could not find a 1.3" version, however, the shown wiring will still work on any setup.

---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Here is how to setup the Physical components:

1. Connect the OLED display to the breadboard, around the centre
<img width="893" height="374" alt="image" src="https://github.com/user-attachments/assets/d5499b16-f15d-40c2-9984-fa99a65de586" />


2. Afterwards, connect the OLED to the Arduino via the breadboard, via jumper wires-
  VCC to a 5V pin
  GND to a GND pin
  SCL to SCL pin
  SDA to SDA pin
<img width="1114" height="386" alt="image" src="https://github.com/user-attachments/assets/c3f03b62-a26e-4050-89ad-75104579b0be" />


3. Connect the buzzer to the breadboard, position on the right of the OLED on the breadboard

4. Via the breadboard and jumper wires, connect the positive side of the buzzer to pin 8 of the arduino, as well as the negative side of the buzzer to pin 9
<img width="1103" height="419" alt="image" src="https://github.com/user-attachments/assets/1400b859-995f-449a-9e32-350612ce40cc" />

5. For the UP push button: Position it on the top right of the breadboard

6. After doing so, connect the UP push button via jumper wires and U-shaped wires (for neatness) to pin 2 of the arduino, and also connecting the negative side of the button to a GND
Arduino
<img width="1123" height="431" alt="image" src="https://github.com/user-attachments/assets/df913f37-7971-482e-b855-24a1ecf262dd" />

8. For the DOWN push button: Position it on the bottom right of the breadboard, under UP

9. After, connect the DOWN push button via jumper and u-shaped wires to pin 3 of the arduino, connecting the negative side to another Arduino GND too
<img width="1130" height="439" alt="image" src="https://github.com/user-attachments/assets/68e6891b-66f0-4f29-8e98-8c8c8aeb6135" />

10. For the LEFT Push button: Poistion it on the left side of the arduino, but in the centre

11. After, connect the LEFT Push Button to the arduino via jumper and u shaped wires, connecting positive to pin 4 and negative to pin 5
<img width="1132" height="452" alt="image" src="https://github.com/user-attachments/assets/a011ec18-ec9d-45fc-a4b3-6dbe2c0df20e" />

12. For the RIGHT Push button, position it on the left side of the arduino, on the centre, but to the right of the left button, for a gaming-console-like layout

13. After, connect the RIGHT push button to the arduino via jumper and u shaped wires, connecting the positive to pin 6 and negative to pin 7
<img width="1118" height="437" alt="image" src="https://github.com/user-attachments/assets/586edfe9-b3f7-4755-b8be-23d2c29c0245" />

14. For the Exiting/Enter game button: Position it on the bottom left of the OLED screen

15. Connect that button to the arduino via jumper and u-shaped wires, so that the positive side is connected to pin 10, and the negative connects to pin 11
<img width="1112" height="446" alt="image" src="https://github.com/user-attachments/assets/12618ac8-c0e8-4b94-89dc-cf824535bbce" />

16. Afterwards, get your Arduino cable and plug it into your device

17. Once connected, upload the code to it which is inside the Gaming_Console_OLED file

18. BOOM! ur done.

By the end of all of this, the layout of the project should look roughly like it does in the finalPic, and should playout like the video which I have uploaded to my YouTube channel: https://www.youtube.com/watch?v=WRdonJKs50Y
