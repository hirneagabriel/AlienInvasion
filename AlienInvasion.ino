#include<LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>
// lcd pins
const int RS = 13;
const int enable = 10;
const int d4 = A2;
const int d5 = A3;
const int d6 = A4;
const int d7 = A5;
const int contrastPin = 5;
const int ledDisplayPin = 6;

// joystick pins
const int VRx = A0;
const int VRy = A1;
const int SW = 2;

//matrix pins
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 7;

// button pin
const int shootButton = 3;
// buzzer pin
const int buzzerPin = 8;
const int buzzerShootDuration = 200;
const int buzzerLaserDuration = 300;
const int buzzerShortDuration = 100;
// variables for joystick
const int minThreshold = 300;
const int maxThreshold = 600;
const int minThresholdMenu = 200;
const int maxThresholdMenu = 800;
int xValue = 0;
int yValue = 0;
bool isMovedX = false;
bool isMovedY = false;
// variables for continuing movement of the ship with one move of the joytick
long int lastMoved = 0;
int  moveInterval = 100;

// matrix and lcd initializations
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// buttons debounce system variables
volatile unsigned long lastPress = 0;
const int debounceTime = 100;
// data read from the joytick button
int SWData = 0;
// data read from the button
int shootButtonData = 0;
// index of the current menu (ex: main menu is 0)
int activeMenu = 0;
// score get from current game sesion
int currentScore = 0;
const int nameLenght = 3;
const int maxDificulty = 3;
int livesLeft = 3;

// variable use for first save in EEPROM
bool firstSave = false;

// data stored in eeprom
// top 3 names
char scoreNames[4][4] = {
  "AAA", "AAA", "AAA"
};
// top 3 scores
int scores[] = {
  0, 0, 0
};
// lcd and matrix controls
int currentBrightness = 100;
int currentContrast = 50;
int matrixIntensity = 2;
// other settings
char username[nameLenght + 1] = "AAA";
int activeDificulty = 1;
// end of variables stored in eeprom
// set to true when lcd need update
bool needsLCDUpdate = true;
// images for 8x8 matrix used for menu
const byte images[][8] {
  {
    B01000010,
    B00100100,
    B00111100,
    B01111110,
    B11011011,
    B11111111,
    B10100101,
    B00000000
  },
  {
    B00010100,
    B00011101,
    B00010101,
    B11100000,
    B10000000,
    B11101100,
    B00101000,
    B11101100
  },
  {
    B00101000,
    B00101000,
    B00010000,
    B00010000,
    B00010000,
    B00010000,
    B00101000,
    B00101000
  },
  {
    B00011000,
    B00111100,
    B01011010,
    B01111110,
    B01111110,
    B00100100,
    B01111110,
    B10100101
  },
  {
    B00000000,
    B00000000,
    B00100100,
    B00100100,
    B00000000,
    B00111100,
    B01000010,
    B00000000
  },
  {
    B00000000,
    B00000000,
    B00100100,
    B00100100,
    B00000000,
    B01000010,
    B00111100,
    B00000000
  },
  {
    B00000000,
    B00110000,
    B00111000,
    B00111100,
    B00111110,
    B00111100,
    B00111000,
    B00110000
  }
};
// custom lcd characters
byte arrow[] = {
  B00000,
  B00100,
  B00110,
  B00111,
  B00110,
  B00100,
  B00000,
  B00000
};
byte heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};
byte arrowDown[] = {
  B00000,
  B00000,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};
byte arrowUp[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B00000,
  B00000
};

//music variables
// notes lots of notes
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// change this to make the song slower or faster
int tempo = 210;

// music used in the intro
const int melody[] PROGMEM = {
  // At Doom's Gate (E1M1)
  // Score available at https://musescore.com/pieridot/doom

  NOTE_E2, 8, NOTE_E2, 8, NOTE_E3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_D3, 8, NOTE_E2, 8, NOTE_E2, 8, //1
  NOTE_C3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_AS2, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_B2, 8, NOTE_C3, 8,
  NOTE_E2, 8, NOTE_E2, 8, NOTE_E3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_D3, 8, NOTE_E2, 8, NOTE_E2, 8,
  NOTE_C3, 8, NOTE_E2, 8, NOTE_E2, 8, NOTE_AS2, -2,
};


// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


//game global values


int matrixSize = 7;
// set to true when the game start
bool isPlaying = false;
bool gameIsReadytoPlay = true;
// true when a score is in top 3
bool isNewHighScore;
// nr of lasers disponible to shoot. at the start of the game is 0.
volatile int nrOfSpecialLasers = 0;
// increase this to get lasers drop more often
const int percentOfLuckForLaser = 4;

// game variables used only in
// ship collum index
int ship = 0;
// blink interval when imortal
const int blinkDelay = 100;
// set to true when ship is not vulnerable
bool isImortal = true;
const int imortalityDuration = 3000;
long int imortalityTimer = 0;
// set to true when matrix needs to be update
bool needScreenUpdate = false;
// timers
long int timer = 0;
long int timerBullet = 0;
// change this make bullet faster or slower
const int bulletSpeed = 30;
// the duration of the enemy's standing still. depends on the dificulty
int frame = 1200;
const int frameEazy = 1200;
const int frameMedium = 800;
const int frameHard = 700;
int frameCounter = 0;
// the number of frames after which the duration of a frame will decrease. depends on the dificulty
int increaseSpeed = 3;
const int increaseSpeedEazy = 3;
const int increaseSpeedMedium = 4;
const int increaseSpeedHard = 5;
// constant used to decrese the frame speed
const float speedConstant = 1.02;
// score increse deferently depending on the dificulty
int scoreByDificulty;
// int generated randomly
int randomInt;
int randomIntLaser;
// each enemy will move on columns only
// if the column is !=0 an enemy will spawn on that column, the value on each column represents the row on witch the enemy is sitting
int enemyColumns[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
// if an enemy is shoot is a chance that an laser is droped
// laser work like the enemys.
int laserColumns[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
// bullets work like the enemys.
volatile int shootColumns[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
// abstract class for the menu
// contains functions that are called by the joystick and a display fuction that displays stuf on the lcd
class Menu {
  public:
    // called by the joystick
    virtual void onLeft() {};
    virtual void onRight() {};
    virtual void onUp() {};
    virtual void onDown() {};
    virtual void onPress() {};
    // display stuff on the lcd
    virtual void displayMenu(LiquidCrystal& lcd) = 0;
    // display images to the 8x8 matrix
    void displayImage(const byte* image) {
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          lc.setLed(0, i, j, bitRead(image[i], 7 - j));
        }
      }
    }
};

// main menu class
class MainMenu : public Menu {
    // used to show data on the lcd
    String menu[5] = {
      "Play", "Highscore", "Settings", "About"
    };
    // index controled by the joystick to point to a certain menu from the string array
    int pointingArrow = 0;
  public:
    // joystick moves up index get's smaller
    void onUp() {
      pointingArrow -= 1;
      if (pointingArrow < 0) {
        pointingArrow = 0;
      }
    }
    // joystick moves down index get's bigger
    void onDown() {
      pointingArrow += 1;
      if (pointingArrow > 3) {
        pointingArrow = 3;
      }
    }
    // on press the menu change to the selected menu
    void onPress() {
      switch (pointingArrow) {
        case 0:
          // play menu
          activeMenu = 1;
          break;
        case 1:
          // hiscore menu
          activeMenu = 2;
          break;
        case 2:
          // settings menu
          activeMenu = 3;
          break;
        case 3:
          // about menu
          activeMenu = 4;
          break;
        default:
          break;
      }
    }
    // display information on the lcd
    void displayMenu(LiquidCrystal& lcd) {
      lcd.clear();
      // display on the matrix an image
      displayImage(images[0]);
      //if index is not last display on the first and second row index menu and index + 1 menu
      if (pointingArrow < 3) {
        lcd.write(byte(0));
        lcd.print(menu[pointingArrow]);
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(menu[pointingArrow + 1]);
      }
      // if index is last display on the first and second row index - 1 menu and index menu
      if (pointingArrow == 3) {
        lcd.print(" ");
        lcd.print(menu[pointingArrow - 1]);
        lcd.setCursor(0, 1);
        // → arrow
        lcd.write(byte(0));
        lcd.print(menu[pointingArrow]);
        lcd.setCursor(15, 0);
        // ↑ arrow
        lcd.write(byte(1));
      }
      // display at the end of each row an helping arrow to suggest what moves can joystick make
      if (pointingArrow == 0) {
        lcd.setCursor(15, 1);
        // ↓ arrow
        lcd.write(byte(2));
      }
      if (pointingArrow > 0 && pointingArrow < 3) {
        lcd.setCursor(15, 1);
        lcd.write(byte(2));
        lcd.setCursor(15, 0);
        lcd.write(byte(1));
      }

    }
};

// play menu class
class PlayMenu : public Menu {
  public:
    // on press of joystick change state from ready to play to playing or form not ready to play to ready to play
    void onPress() {
      if (gameIsReadytoPlay == false) {
        gameIsReadytoPlay = true;
      } else {
        if (isPlaying == false) {
          isPlaying = true;
          // at the start of the game reset game lives and score
          livesLeft = 3;
          currentScore = 0;
        }
      }
    }
    // exit to my menu if is ok to do so
    void onLeft() {
      if (isPlaying == false && gameIsReadytoPlay == true) {
        activeMenu = 0;
      }
    }
    // display stuf on the lcd
    void displayMenu(LiquidCrystal& lcd) {
      // if game has started
      if (isPlaying == true) {
        lcd.clear();
        // print the username
        lcd.print(username);
        lcd.print(":");
        // for nr of lives left draw one heart
        for (int i = 0; i < livesLeft; i++)
          lcd.write(byte(3));
        lcd.setCursor(8, 0);
        // print current dificulty
        if (activeDificulty == 1) {
          lcd.print("Eazy");
        }
        if (activeDificulty == 2) {
          lcd.print("Medium");
        }
        if (activeDificulty == 3) {
          lcd.print("Hard");
        }

        lcd.setCursor(0, 1);
        // print the score
        lcd.print("SC: ");
        lcd.print(currentScore);
        lcd.setCursor(8, 1);
        // print nr of lasers
        lcd.print("Laser: ");
        lcd.print(nrOfSpecialLasers);
      }
      else {
        // if game is ready to be played
        if (gameIsReadytoPlay == true) {
          // draw an image on the matrix
          displayImage(images[6]);
          lcd.clear();
          // print messages for start of the game
          lcd.print(" Press the btn");
          lcd.setCursor(0, 1);
          lcd.print("   to play");
        } else {
          // if the game is not ready to play and is not playing show the score and a message.
          lcd.clear();
          // if top 3 has changed print a messge and show an image on matrix
          if (isNewHighScore == true) {
            displayImage(images[5]);
            lcd.print("You're in top 3!");
            lcd.setCursor(0, 1);
            lcd.print(username);
            lcd.print("'s SC: ");
            lcd.print(currentScore);
          } else {
            // if top 3 hasn't changed print a messge and show an image on matrix
            displayImage(images[4]);
            lcd.print("  Game over");
            lcd.setCursor(0, 1);
            lcd.print(username);
            lcd.print("'s SC: ");
            lcd.print(currentScore);
          }
        }
      }

    }

};

// Highscore menu class
class HiScoreMenu : public Menu {
    int pointingArrow = 0;
  public:
    // same controls as the main menu
    void onUp() {
      pointingArrow -= 1;
      if (pointingArrow < 0) {
        pointingArrow = 0;
      }

    };
    void onDown() {
      pointingArrow += 1;
      if (pointingArrow > 1) {
        pointingArrow = 1;
      }
    };
    // go to main menu on left
    void onLeft() {
      activeMenu = 0;
    }
    // go to main menu on right
    void onPress() {
      activeMenu = 0;
    }
    void displayMenu(LiquidCrystal& lcd) {
      // show on the matrix an image
      displayImage(images[1]);
      lcd.clear();
      // print leaderboard
      lcd.print(pointingArrow + 1);
      lcd.print(".");
      lcd.print(scoreNames[pointingArrow]);
      lcd.print(": ");
      lcd.print(scores[pointingArrow]);
      lcd.setCursor(0, 1);
      lcd.print(pointingArrow + 2);
      lcd.print(".");
      lcd.print(scoreNames[pointingArrow + 1]);
      lcd.print(": ");
      lcd.print(scores[pointingArrow + 1]);
      // print helping up and down arrows at the end of the rows
      if (pointingArrow == 0) {
        lcd.setCursor(15, 1);
        lcd.write(byte(2));
      }
      else {
        lcd.setCursor(15, 0);
        lcd.write(byte(1));
      }

    }

};

// settings Menu class
class SettingsMenu : public Menu {
    // blink when editing someting
    int blinkDelay = 500;
    // settings to be changed
    String menu[6] = {
      "Username", "Dificulty", "Contrast", "LCD Bright", "M Bright", "ResetHiScore"
    };
    // bools set to true when an settings is in edit mode
    bool isEditing = false;
    bool editName = false;
    bool editDificulty = false;
    bool editContrast = false;
    bool editLedBrightness = false;
    bool editMatrixBrightness = false;
    bool scoreReset = false;
    int charPosition = 0;
    int pointingArrow = 0;
  public:
    // store changed data in eeprom
    void storeSettingsData() {
      int eeAddress = 0;
      firstSave = true;
      EEPROM.put(eeAddress, firstSave);
      eeAddress += sizeof(bool);
      EEPROM.put(eeAddress, username);
      eeAddress += sizeof(username);
      EEPROM.put(eeAddress, activeDificulty);
      eeAddress += sizeof(int);
      EEPROM.put(eeAddress, currentContrast);
      eeAddress += sizeof(int);
      EEPROM.put(eeAddress, currentBrightness);
      eeAddress += sizeof(int);
      EEPROM.put(eeAddress, matrixIntensity);
      eeAddress += sizeof(int);
      for (int i = 0; i < 3; i++) {
        EEPROM.put(eeAddress, scores[i]);
        eeAddress += sizeof(int);
      }
      for (int i = 0; i < 3; i++) {
        EEPROM.put(eeAddress, scoreNames[i]);
        eeAddress += sizeof(scoreNames[i]);
      }
    }
    // on the press of the joystick
    void onPress() {
      // the state is changed to editing or not editing
      isEditing = !isEditing;
      // change the state of the selected variable
      if (pointingArrow == 0) {
        editName = !editName;
      }
      if (pointingArrow == 1) {
        editDificulty = !editDificulty;
      }
      if (pointingArrow == 2) {
        editContrast = !editContrast;
      }
      if (pointingArrow == 3) {
        editLedBrightness = !editLedBrightness;
      }
      if (pointingArrow == 4) {
        editMatrixBrightness = !editMatrixBrightness;
      }
      // if the reset leaderboard setting is selected
      if (pointingArrow == 5) {
        scoreReset = true;
        // reset each name with "AAA" and each score with 0
        for (int i = 0; i < 3; i++) {
          strcpy(scoreNames[i], "AAA");
          scores[i] = 0;
        }
        isEditing = !isEditing;
      }
    }
    void onLeft() {
      // if editing mode is not activated exit to main menu
      if (isEditing == false) {
        activeMenu = 0;
        storeSettingsData();
      }
      // if username is in edit mode change the index character to be changed
      if (editName == true) {
        charPosition -= 1;
        if (charPosition < 0) {
          charPosition = nameLenght - 1;
        }
      }
      if (editDificulty == true) {
        activeDificulty -= 1;
        if (activeDificulty <= 0) {
          activeDificulty = maxDificulty;
        }
      }
      if (editContrast == true) {
        currentContrast -= 10;
        if (currentContrast < 0) {
          currentContrast = 0;
        }
        // update the variable for live change on the lcd
        analogWrite(contrastPin, currentContrast);
      }
      if (editLedBrightness == true) {
        currentBrightness -= 10;
        if (currentBrightness < 0) {
          currentBrightness = 0;
        }
        // update the variable for live change on the lcd
        analogWrite(ledDisplayPin, currentBrightness);
      }
      if (editMatrixBrightness == true) {
        matrixIntensity -= 1;
        if (matrixIntensity < 0) {
          matrixIntensity = 0;
        }
        // update the variable for live change on the matrix
        lc.setIntensity(0, matrixIntensity);
      }
    }
    // like the onLeft function
    void onRight() {
      if (editName == true) {
        charPosition += 1;
        if (charPosition >= nameLenght) {
          charPosition = 0;
        }
      }
      if (editDificulty == true) {
        activeDificulty += 1;
        if (activeDificulty > maxDificulty) {
          activeDificulty = 1;
        }
      }
      if (editContrast == true) {
        currentContrast += 10;
        if (currentContrast > 80) {
          currentContrast = 80;
        }
        analogWrite(contrastPin, currentContrast);
      }
      if (editLedBrightness == true) {
        currentBrightness += 10;
        if (currentBrightness > 255) {
          currentBrightness = 255;
        }
        analogWrite(ledDisplayPin, currentBrightness);
      }
      if (editMatrixBrightness == true) {
        matrixIntensity += 1;
        if (matrixIntensity > 15) {
          matrixIntensity = 15;
        }
        lc.setIntensity(0, matrixIntensity);
      }


    }

    void onDown() {
      // if not in edit mode change the index of the pointing arrow
      if (isEditing == false) {
        pointingArrow += 1;
        if (pointingArrow > 5) {
          pointingArrow = 5;
        }
      }
      // if name is in editing mode change the current character
      if (editName == true) {
        username[charPosition] = username[charPosition] + 1;
        if (username[charPosition] > 'Z') {
          username[charPosition] = 'A';
        }
      }
    }

    void onUp() {
      // if not in edit mode change the index of the pointing arrow
      if (isEditing == false) {
        pointingArrow -= 1;
        if (pointingArrow < 0) {
          pointingArrow = 0;
        }
      }
      // if name is in editing mode change the current character
      if (editName == true) {
        username[charPosition] = username[charPosition] - 1;
        if (username[charPosition] < 'A') {
          username[charPosition] = 'Z';
        }
      }
    }

    void displayMenu(LiquidCrystal& lcd) {
      displayImage(images[2]);
      lcd.clear();
      if (pointingArrow < 5) {
        // print on the first row
        lcd.print(" ");
        lcd.print(menu[pointingArrow]);
        lcd.print(" ");
        if (pointingArrow == 0) {
          // add blinking effect to the selected character if the name is in edit mode
          if (editName == true) {
            if ((millis() % (2 * blinkDelay)) > blinkDelay) {
              for (int i = 0 ; i < 3; i++) {
                if (charPosition == i) {
                  lcd.print("_");
                }
                else {
                  lcd.print(username[i]);
                }
              }

            }
            else {
              lcd.print(username);
            }
          } else {
            lcd.print(username);
          }
        }
        if (pointingArrow == 1)
          lcd.print(activeDificulty);
        if (pointingArrow == 2)
          lcd.print(currentContrast);
        if (pointingArrow == 3)
          lcd.print(currentBrightness);
        if (pointingArrow == 4)
          lcd.print(matrixIntensity);
        // print on the second row
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(menu[pointingArrow + 1]);
        lcd.print(" ");
        if (pointingArrow == 0)
          lcd.print(activeDificulty);
        if (pointingArrow == 1)
          lcd.print(currentContrast);
        if (pointingArrow == 2)
          lcd.print(currentBrightness);
        if (pointingArrow == 3)
          lcd.print(matrixIntensity);
        lcd.setCursor(0, 0);
      }
      if (pointingArrow == 5) {
        // print on the first row
        lcd.print(" ");
        lcd.print(menu[pointingArrow - 1]);
        lcd.print(" ");
        lcd.print(matrixIntensity);
        // print on the second row
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(menu[pointingArrow]);
        lcd.setCursor(15, 0);
        lcd.write(byte(1));
        lcd.setCursor(0, 1);
      }

      // print arrows an blink if in editing mode
      if (isEditing == false) {
        lcd.write(byte(0));
      } else {
        if ((millis() % (2 * blinkDelay)) < blinkDelay) {
          lcd.write(byte(0));
        } else {
          lcd.print(" ");
        }
      }

      if (pointingArrow == 0) {
        lcd.setCursor(15, 1);
        lcd.write(byte(2));
      }
      if (pointingArrow > 0 && pointingArrow < 5) {
        lcd.setCursor(15, 1);
        lcd.write(byte(2));
        lcd.setCursor(15, 0);
        lcd.write(byte(1));
      }
      // if the reset function has been activated show a custom message
      if (scoreReset == true) {
        lcd.clear();
        lcd.print("Leaderboard has");
        lcd.setCursor(0, 1);
        lcd.print("been reset!");
        scoreReset = false;
        delay(1000);
        needsLCDUpdate = true;
      }
      // if editing lcd will constantly be updated for blinking effect
      if (isEditing == true) {
        needsLCDUpdate = true;
        delay(50);
      }
    }
};

// about menu class
class AboutMenu : public Menu {
    long int timerScroll = 0;
    int intervalLeft = 0;
    int intervalRight = 16;
    int scrollSpeed = 400;
    String message = "Made by Hirnea Gabriel. Github link: https://github.com/hirneagabriel/AlienInvasion";
  public:

    void onLeft() {
      activeMenu = 0;
    }
    // scroling text menu
    String scrollText(String text) {
      String result;
      // add some spaces to the message (15 spaces are recomended)
      String proccesString = "               " + text + "               ";
      // get 16 characters and increase the indexInterval
      result = proccesString.substring(intervalRight, intervalLeft);
      intervalLeft++;
      intervalRight++;
      // if the end is reached start from the beginning
      if (intervalRight > proccesString.length()) {
        intervalRight = 16;
        intervalLeft = 0;
      }
      return result;
    }
    void displayMenu(LiquidCrystal& lcd) {
      // diplay image in the matrix
      displayImage(images[3]);
      // refresh at the scroll interval
      if (millis() - timerScroll > scrollSpeed) {
        lcd.clear();
        timerScroll = millis();
        lcd.print(" Alien Invasion");
        lcd.setCursor(0, 1);
        // show the result to the second row
        lcd.print(scrollText(message));
        timerScroll = millis();
      }
    }

};

// instance of the classes
MainMenu mainMenu;
HiScoreMenu hiScoreMenu;
SettingsMenu settingsMenu;
PlayMenu playMenu;
AboutMenu aboutMenu;

// set an index for each menu class and return the instance of that class
Menu& currentMenu() {
  if (activeMenu == 0)
    return mainMenu;
  if (activeMenu == 2)
    return hiScoreMenu;
  if (activeMenu == 3)
    return settingsMenu;
  if (activeMenu == 1)
    return playMenu;
  if (activeMenu == 4)
    return aboutMenu;
}

// function for the joystick that controls the menu
void jsForMenu() {
  xValue = analogRead(VRx);
  yValue = analogRead(VRy);
  // call curentMenu method depending on the move of the joystick
  if (yValue <= minThresholdMenu && isMovedY == false) {
    isMovedY = true;
    currentMenu().onDown();
    needsLCDUpdate = true;
    joystickSound();
  }

  if (yValue >= maxThresholdMenu && isMovedY == false) {
    isMovedY = true;
    currentMenu().onUp();
    needsLCDUpdate = true;
    joystickSound();
  }

  if (xValue <= minThresholdMenu && isMovedX == false) {
    isMovedX = true;
    currentMenu().onRight();
    needsLCDUpdate = true;
    joystickSound();
  }

  if (xValue >= maxThresholdMenu && isMovedX == false) {
    isMovedX = true;
    currentMenu().onLeft();
    needsLCDUpdate = true;
    joystickSound();
  }

  if (xValue > minThresholdMenu && xValue < maxThresholdMenu) {
    isMovedX = false;
  }

  if (yValue > minThresholdMenu && yValue < maxThresholdMenu) {
    isMovedY = false;
  }
}

// get all the data form eeprom
void getData() {
  int address = 0;
  EEPROM.get(address, firstSave);
  address += sizeof(bool);
  if (firstSave == true) {
    EEPROM.get(address, username);
    address += sizeof(username);
    EEPROM.get(address, activeDificulty);
    address += sizeof(int);
    EEPROM.get(address, currentContrast);
    address += sizeof(int);
    EEPROM.get(address, currentBrightness);
    address += sizeof(int);
    EEPROM.get(address, matrixIntensity);
    address += sizeof(int);

    for (int i = 0; i < 3; i++) {
      EEPROM.get(address, scores[i]);
      address += sizeof(int);
    }
    for (int i = 0; i < 3; i++) {
      EEPROM.get(address, scoreNames[i]);
      address += sizeof(scoreNames[i]);
    }
  }
}

// saving the highscore data
void putHiScoreData() {
  int address = 13;
  for (int i = 0; i < 3; i++) {
    EEPROM.put(address, scores[i]);
    address += sizeof(int);
  }
  for (int i = 0; i < 3; i++) {
    EEPROM.put(address, scoreNames[i]);
    address += sizeof(scoreNames[i]);
  }
}


// function for controling the ship with joystick
// control the ship on the last row of the matrix
void jsForGame() {
  xValue = analogRead(VRx);
  if (xValue < minThreshold) {
    if (ship < matrixSize) {
      ship++;
      needScreenUpdate = true;
    }
    else {
      ship = matrixSize ;
    }

  }
  if (xValue > maxThreshold) {
    if (ship > 0) {
      ship--;
      needScreenUpdate = true;
    }
    else {
      ship = 0;
    }

  }
}

// return true if top 3 is changed
bool checkHighScore() {
  bool isNewTop3 = false;
  // for each leaderboard score check if current score is bigger.
  for (int i = 0 ; i < 3; i++) {
    // update the leaderboard
    if (scores[i] < currentScore) {
      for (int j = 2 ; j >= i ; j--) {
        scores[j] = scores[j - 1];
        strcpy(scoreNames[j], scoreNames[j - 1]);
      }
      scores[i] = currentScore;
      strcpy(scoreNames[i], username);
      // save data in eeprom
      putHiScoreData();
      isNewTop3 = true;
      break;
    }
  }
  return isNewTop3;
}

// checking if player have lives left
void checkLivesLeft() {
  // if ship not imortal decrease nr of lives
  if (isImortal == false) {
    livesLeft--;
    needsLCDUpdate = true;
    // if no lives left  and check if the score
    if (livesLeft < 0) {
      isNewHighScore = checkHighScore();
      isPlaying = false;
      // game is stopped
      gameIsReadytoPlay = false;
    }
    else {
      // else ship becomes imortal for 3 seconds
      isImortal = true;
      imortalityTimer = millis();
    }
  }
}

// display stuf on the matrix
void displayGame() {
  lc.clearDisplay(0);
  // if ship is imortal blink the ship dot for 3 seconds
  if (isImortal == false) {
    lc.setLed(0, matrixSize, ship, true);
  } else {
    if (millis() - imortalityTimer < imortalityDuration) {
      if ((millis() % (2 * blinkDelay)) < blinkDelay) {
        lc.setLed(0, matrixSize, ship, true);
      }
      else {
        lc.setLed(0, matrixSize, ship, false);
      }
    } else {
      isImortal = false;
    }
  }
  // show the enemys
  for (int i = 0; i <= matrixSize; i++) {
    // show the enemys
    if (enemyColumns[i] > 0) {
      lc.setLed(0, enemyColumns[i] - 2, i, true);
      lc.setLed(0, enemyColumns[i] - 3, i, true);
    }
    // show the bullet
    if (shootColumns[i] > 0) {
      lc.setLed(0, shootColumns[i], i, true);
    }
    // show the laser drops
    if (laserColumns[i] > 0) {
      lc.setLed(0, laserColumns[i] - 2, i, true);
    }
  }
  // if the game is over clearDisplay
  if (isPlaying == false) {
    lc.clearDisplay(0);
  }
}

// game logic

void game() {
  // for each frame
  if (millis() - timer > frame) {
    // increase the score
    currentScore += scoreByDificulty;
    // update the screen
    needsLCDUpdate = true;
    timer = millis();
    // at increaseSpeed frames
    if (frameCounter == increaseSpeed) {
      // the frame time is decresed
      frame = frame / speedConstant;
      // set the frameCounter to 0
      frameCounter = 0;
    }
    // chose a random int from 0 to matrixSize
    randomInt = random(0, matrixSize + 1);
    // if no obj in the radom column create one object in  row 1
    if (enemyColumns[randomInt] == 0) {
      enemyColumns[randomInt] = 1;
    }
    // increase the frameCounter
    frameCounter++;


    for (int i = 0; i <= matrixSize; i++) {
      // enemy out of view
      if (enemyColumns[i] == 10) {
        //set the enemy row to 0 so it can be use again and decrese one life
        enemyColumns[i] = 0;
        checkLivesLeft();
      }
      // same witch laser drop
      if (laserColumns[i] == 9) {
        laserColumns[i] = 0;
      }
      // else lower the enemy and the laser drop
      if (enemyColumns[i] != 0) {
        enemyColumns[i]++;
      }

      if (laserColumns[i] != 0) {
        laserColumns[i]++;
      }

    }

    needScreenUpdate = true;
  }
  // bullet is moving at a constant speed until it collides with a enemy
  if (millis() - timerBullet > bulletSpeed) {
    for (int i = 0; i <= matrixSize; i++) {
      // if a bullet exist
      if (shootColumns[i] != 0) {
        // check collision with enemy
        if (shootColumns[i] < enemyColumns[i]) {
          // if collision exist get a random int from 0 to 100
          randomIntLaser = random(0, 100);
          // if the int is lower than 4
          if (randomIntLaser <= percentOfLuckForLaser) {
            // drop a laser
            laserColumns[i] = enemyColumns[i];
          }
          // destroy te bullet and the enemy
          shootColumns[i] = 0;
          enemyColumns[i] = 0;
        } else {
          // else the bullet continue
          shootColumns[i]--;
        }
      }
    }
    timerBullet = millis();
    needScreenUpdate = true;
  }
  // enemy collide with the ship
  if (enemyColumns[ship] == 10 || enemyColumns[ship] == 9) {
    // check if game is over
    checkLivesLeft();
    enemyColumns[ship] = 0;
  }
  // if ship colides with laser drop get one laser ability
  if (laserColumns[ship] == 9) {
    // 9 max lasers
    if (nrOfSpecialLasers < 9) {
      nrOfSpecialLasers++;
    }
    laserColumns[ship] = 0;
  }

}
// game variable initialization an the end of each play
void gameInitialization() {
  for (int i = 0; i <= matrixSize; i++) {
    enemyColumns[i] = 0;
    shootColumns[i] = 0;
  }
  if (activeDificulty == 1) {
    frame = frameEazy;
    increaseSpeed = increaseSpeedEazy;
    scoreByDificulty = 1;
  }
  if (activeDificulty == 2) {
    frame = frameMedium;
    increaseSpeed = increaseSpeedMedium;
    scoreByDificulty = 2;
  }
  if (activeDificulty == 3) {
    frame = frameHard;
    increaseSpeed = increaseSpeedHard;
    scoreByDificulty = 3;
  }
  nrOfSpecialLasers = 0;
}

// intrerupt function for shooting at the press of the button
void shoot() {
  volatile unsigned long intrerruptTime = 0;
  intrerruptTime = millis();
  if (intrerruptTime - lastPress > debounceTime) {
    if (isPlaying == true) {
      // shoot a bullet and make a sound
      if (shootColumns[ship] == 0) {
        shootColumns[ship] = 7;
        buttonSound();
      }
    }
  }
  lastPress = millis();
}

// joystick button function
void buttonIsPressed() {
  volatile unsigned long intrerruptTime = 0;
  intrerruptTime = millis();
  if (intrerruptTime - lastPress > debounceTime) {
    // call the current menu function
    currentMenu().onPress();
    needsLCDUpdate = true;
    joystickButtonSound();
    if (isPlaying == true) {
      // deploy a super laser and make a sound
      if (nrOfSpecialLasers != 0) {
        for (int i = 0; i <= matrixSize; i++) {
          shootColumns[i] = 7;
        }
        //decrese nr of lasers
        nrOfSpecialLasers--;
      }
    }
  }
  lastPress = millis();
}

// sound when moving the joystick
void joystickSound() {
  tone(buzzerPin, NOTE_GS4, buzzerShortDuration);
}
// sound when pressing the button
void buttonSound() {
  tone(buzzerPin, NOTE_G5, buzzerShootDuration);
}
// sound when pressing the joystick
void joystickButtonSound() {
  if (isPlaying == false) {
    tone(buzzerPin, NOTE_DS5, buzzerShortDuration);
  }
  else {
    if (nrOfSpecialLasers != 0) {
      tone(buzzerPin, NOTE_C6, buzzerLaserDuration);
    }
  }
}
// intro music function
void introMusic() {
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = pgm_read_word_near(melody + thisNote + 1);
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzerPin, pgm_read_word_near(melody + thisNote), noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzerPin);
  }
}

// function that helps with button debouncing
void debounceButton() {
  // if the button is down reset the lastPress variable
  SWData = digitalRead(SW);
  shootButtonData = digitalRead(shootButton);
  if (SWData == 0 || shootButtonData == 0) {
    lastPress = millis();
  }
}



void setup() {

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  //putData();

  // get data from eeprom
  getData();
  // all pins initializations and stuff
  lcd.begin(16, 2);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(contrastPin, OUTPUT);
  pinMode(ledDisplayPin, OUTPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(shootButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(SW), buttonIsPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(shootButton), shoot, FALLING);
  analogWrite(contrastPin, currentContrast);
  analogWrite(ledDisplayPin, currentBrightness);
  // creating new characters
  lcd.createChar(0, arrow);
  lcd.createChar(1, arrowUp);
  lcd.createChar(2, arrowDown);
  lcd.createChar(3, heart);
  lcd.clear();

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixIntensity);
  lc.clearDisplay(0);
  // intro message
  lcd.print(" AlienInvasion");
  introMusic();
  // initialize the game variabels
  gameInitialization();
  Serial.begin(9600);
}



void loop() {
  // eliminate debounce button
  debounceButton();
  // control the menu with joystick
  if (isPlaying == false) {
    jsForMenu();
  }
  // if the screen needs to be update it's going to be updated
  // exception for about menu that always needs to be updated
  if (needsLCDUpdate == true || activeMenu == 4) {
    needsLCDUpdate = false;
    currentMenu().displayMenu(lcd);

  }

  // initialize the game if the game is over
  if (isPlaying == false) {
    gameInitialization();
  }
  // game is running
  if (isPlaying == true) {
    // move ship at constant speed
    if (millis() - lastMoved > moveInterval) {
      jsForGame();
      lastMoved = millis();
    }
    game();
    if (needScreenUpdate == true) {
      displayGame();
      needScreenUpdate = false;
    }
  }

}
