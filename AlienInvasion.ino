#include<LiquidCrystal.h>
#include "LedControl.h" //  need the library
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
int SWData = 0;
int shootButtonData = 0;
//
int currentState = 0;
int currentScore = 0;
const int nameLenght = 3;
const int maxDificulty = 3;
int livesLeft = 3;

// eeprom variables
bool firstSave = false;
int eeAddress = 0;

//data stored in eeprom name suggest what each control
char scoreNames[4][4] = {
  "AAA", "AAA", "AAA"
};
int scores[] = {
  0, 0, 0
};
int currentBrightness = 100;
int currentContrast = 130;
int matrixIntensity = 2;
char username[nameLenght + 1] = "AAA";
int activeDificulty = 1;

// set to true when lcd need update
bool needsLCDUpdate = true;

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
int tempo = 225;

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
bool isPlaying = false;
bool gameIsReadytoPlay = true;
bool isNewHighScore = false;


// game variables used only in
int ship = 0;
const int blinkDelay = 100;
bool isImortal = true;
const int imortalityDuration = 3000;
long int imortalityTimer = 0;
// set to true when matrix needs to be update
bool needScreenUpdate = false;
long int timer = 0;
long int timerBullet = 0;
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
// each enemy will move on columns only
// if the column is !=0 an enemy will spawn on that column, the value on each column represents the row on whitch the enemy is sitting
int enemyColumns[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
// bullets have the same proprietes as the enemys.
volatile int shootColumns[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
// abstract class for the menu
// contains functions that are called by the joystick and a display fuction that displays stuf on the lcd
class Menu {
  public:
    virtual void onLeft() {};
    virtual void onRight() {};
    virtual void onUp() {};
    virtual void onDown() {};
    virtual void onPress() {};
    virtual void displayMenu(LiquidCrystal& lcd) = 0;
};

// main menu class
class MainMenu : public Menu {
    String menu[5] = {
      "Play", "Highscore", "Settings", "About"
    };
    int pointingArrow = 0;
  public:
    void onUp() {
      pointingArrow -= 1;
      if (pointingArrow < 0) {
        pointingArrow = 0;
      }
    }
    void onDown() {
      pointingArrow += 1;
      if (pointingArrow > 3) {
        pointingArrow = 3;
      }
    }
    void onPress() {
      switch (pointingArrow) {
        case 0:
          currentState = 1;
          break;
        case 1:
          currentState = 2;
          break;
        case 2:
          currentState = 3;
          break;
        case 3:
          currentState = 4;
          break;
        default:
          break;
      }
    }
    void displayMenu(LiquidCrystal& lcd) {
      lcd.clear();

      if (pointingArrow < 3) {
        lcd.write(byte(0));
        lcd.print(menu[pointingArrow]);
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(menu[pointingArrow + 1]);
      }
      if (pointingArrow == 3) {
        lcd.print(" ");
        lcd.print(menu[pointingArrow - 1]);
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        lcd.print(menu[pointingArrow]);
        lcd.setCursor(15, 0);
        lcd.write(byte(1));
      }
      if (pointingArrow == 0) {
        lcd.setCursor(15, 1);
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
    long int startTime;
    bool gameOver = false;
  public:
    void onPress() {
      if (gameIsReadytoPlay == false) {
        gameIsReadytoPlay = true;
      } else {
        if (isPlaying == false) {
          startTime = millis();
          isPlaying = true;
          livesLeft = 3;
          currentScore = 0;
        }
      }
    }

    void onLeft() {
      if (isPlaying == false) {
        currentState = 0;
      }
    }

    void displayMenu(LiquidCrystal& lcd) {

      if (isPlaying == true) {
        lcd.clear();
        lcd.print(" ");
        for (int i = 0; i < livesLeft; i++)
          lcd.write(byte(3));
        lcd.setCursor(8, 0);
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
        lcd.print("Score: ");
        lcd.print(currentScore);
      }
      else {
        if (gameIsReadytoPlay == true) {
          lcd.clear();
          lcd.print(" Press the btn");
          lcd.setCursor(0, 1);
          lcd.print("   to play");
        } else {
          lcd.clear();
          if (isNewHighScore == true) {
            lcd.print("You're in top 3!");
            lcd.setCursor(0, 1);
            lcd.print("Score: ");
            lcd.print(currentScore);
          } else {
            lcd.print("  Game over");
            lcd.setCursor(0, 1);
            lcd.print("Score: ");
            lcd.print(currentScore);
          }
        }
      }

    }

};

// Highscore menu class
class HiScoreMenu : public Menu {
    int currentLine = 0;
  public:
    void onUp() {
      currentLine -= 1;
      if (currentLine < 0) {
        currentLine = 0;
      }

    };
    void onDown() {
      currentLine += 1;
      if (currentLine > 1) {
        currentLine = 1;
      }
    };
    void onLeft() {
      currentState = 0;
    }
    void onPress() {
      currentState = 0;
    }
    void displayMenu(LiquidCrystal& lcd) {
      lcd.clear();
      lcd.print(currentLine + 1);
      lcd.print(".");
      lcd.print(scoreNames[currentLine]);
      lcd.print(": ");
      lcd.print(scores[currentLine]);
      lcd.setCursor(0, 1);
      lcd.print(currentLine + 2);
      lcd.print(".");
      lcd.print(scoreNames[currentLine + 1]);
      lcd.print(": ");
      lcd.print(scores[currentLine + 1]);
      if (currentLine == 0) {
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
    int blinkDelay = 500;
    String menu[5] = {
      "Username", "Dificulty", "Contrast", "LCD Bright", "M Bright"
    };
    bool isEditing = false;
    bool editName = false;
    bool editDificulty = false;
    bool editContrast = false;
    bool editLedBrightness = false;
    bool editMatrixBrightness = false;
    int charPosition = 0;
    int pointingArrow = 0;
  public:
    void storeSettingsData() {
      eeAddress = 0;
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
    }
    void onPress() {
      isEditing = !isEditing;
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
    }
    void onLeft() {
      if (isEditing == false) {
        currentState = 0;
        storeSettingsData();
      }
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
        analogWrite(contrastPin, currentContrast);
      }
      if (editLedBrightness == true) {
        currentBrightness -= 10;
        if (currentBrightness < 0) {
          currentBrightness = 0;
        }
        analogWrite(ledDisplayPin, currentBrightness);
      }
      if (editMatrixBrightness == true) {
        matrixIntensity -= 1;
        if (matrixIntensity < 0) {
          matrixIntensity = 0;
        }
        lc.setIntensity(0, matrixIntensity);
      }
    }
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
        if (currentContrast > 255) {
          currentContrast = 255;
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
      if (isEditing == false) {
        pointingArrow += 1;
        if (pointingArrow > 4) {
          pointingArrow = 4;
        }
      }
      if (editName == true) {
        username[charPosition] = username[charPosition] + 1;
        if (username[charPosition] > 'Z') {
          username[charPosition] = 'A';
        }
      }
    }
    void onUp() {
      if (isEditing == false) {
        pointingArrow -= 1;
        if (pointingArrow < 0) {
          pointingArrow = 0;
        }
      }
      if (editName == true) {
        username[charPosition] = username[charPosition] - 1;
        if (username[charPosition] < 'A') {
          username[charPosition] = 'Z';
        }
      }
    }
    void displayMatrix(bool lightOnOff) {
      for (int i = 0 ; i <= matrixSize; i++)
        for (int j = 0; j <= matrixSize; j++)
          lc.setLed(0, i, j, lightOnOff);
    }
    void displayMenu(LiquidCrystal& lcd) {
      if (editMatrixBrightness == true)
        displayMatrix(true);
      else {
        displayMatrix(false);
      }
      lcd.clear();
      if (pointingArrow < 4) {
        lcd.print(" ");
        lcd.print(menu[pointingArrow]);
        lcd.print(" ");
        if (pointingArrow == 0) {
          if (editName == true) {
            if ((millis() % (2 * blinkDelay)) > blinkDelay) {
              //lcd.setCursor(10 + charPosition, pointingArrow);
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
      if (pointingArrow == 4) {
        lcd.print(" ");
        lcd.print(menu[pointingArrow - 1]);
        lcd.print(" ");
        lcd.print(currentBrightness);
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(menu[pointingArrow]);
        lcd.print(" ");
        lcd.print(matrixIntensity);
        lcd.setCursor(15, 0);
        lcd.write(byte(1));
        lcd.setCursor(0, 1);
      }


      if (isEditing == false) {
        lcd.write(byte(0));
      } else {
        if ((millis() % (2 * blinkDelay)) < blinkDelay) {
          lcd.write(byte(0));
        } else {
          lcd.print(" ");
        }
      }
      if (editName == true) {
        if ((millis() % (2 * blinkDelay)) > blinkDelay) {
          lcd.setCursor(10 + charPosition, pointingArrow);
          lcd.print("_");
        }
      }
      if (pointingArrow == 0) {
        lcd.setCursor(15, 1);
        lcd.write(byte(2));
      }
      if (pointingArrow > 0 && pointingArrow < 4) {
        lcd.setCursor(15, 1);
        lcd.write(byte(2));
        lcd.setCursor(15, 0);
        lcd.write(byte(1));
      }
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
    String message = "Made by Hirnea Gabriel. Github link: https://github.com/hirneagabriel/AlienInvasion";
  public:

    void onLeft() {
      currentState = 0;
    }

    String scrollText(String text) {
      String result;
      String proccesString = "               " + text + "             ";
      result = proccesString.substring(intervalRight, intervalLeft);
      intervalLeft++;
      intervalRight++;
      if (intervalRight > proccesString.length()) {
        intervalRight = 16;
        intervalLeft = 0;
      }
      return result;
    }
    void displayMenu(LiquidCrystal& lcd) {

      if (millis() - timerScroll > 400) {
        lcd.clear();
        timerScroll = millis();
        lcd.print(" Alien Invasion");
        lcd.setCursor(0, 1);
        lcd.print(scrollText(message));
        timerScroll = millis();
      }
    }


};

// declaration of the classes
MainMenu mainMenu;
HiScoreMenu hiScoreMenu;
SettingsMenu settingsMenu;
PlayMenu playMenu;
AboutMenu aboutMenu;

// function that swich between classes
Menu& currentMenu() {
  if (currentState == 0)
    return mainMenu;
  if (currentState == 2)
    return hiScoreMenu;
  if (currentState == 3)
    return settingsMenu;
  if (currentState == 1)
    return playMenu;
  if (currentState == 4)
    return aboutMenu;
}

// function for the joystick that controls the menu
void jsForMenu() {
  xValue = analogRead(VRx);
  yValue = analogRead(VRy);
  Serial.println(yValue);
  if (yValue <= minThresholdMenu && isMovedY == false) {
    isMovedY = true;
    currentMenu().onDown();
    needsLCDUpdate = true;
  }

  if (yValue >= maxThresholdMenu && isMovedY == false) {
    isMovedY = true;
    currentMenu().onUp();
    needsLCDUpdate = true;
  }

  if (xValue <= minThresholdMenu && isMovedX == false) {
    isMovedX = true;
    currentMenu().onRight();
    needsLCDUpdate = true;
  }

  if (xValue >= maxThresholdMenu && isMovedX == false) {
    isMovedX = true;
    currentMenu().onLeft();
    needsLCDUpdate = true;
  }

  if (xValue >= minThresholdMenu && xValue <= maxThresholdMenu) {
    isMovedX = false;
  }

  if (yValue >= minThresholdMenu && yValue <= maxThresholdMenu) {
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
    eeAddress = address;

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

// checking if the leaderboard is changed and change the leaderboard
bool checkHighScore() {
  for (int i = 0 ; i < 3; i++) {
    if (scores[i] < currentScore) {
      for (int j = 2 ; j >= i ; j--) {
        scores[j] = scores[j - 1];
        strcpy(scoreNames[j], scoreNames[j - 1]);
      }
      scores[i] = currentScore;
      strcpy(scoreNames[i], username);
      putHiScoreData();
      return true;
      break;
    }
  }
  return false;
}

// checking if player have lives left
void checkLivesLeft() {
  if (isImortal == false) {
    livesLeft--;
    needsLCDUpdate = true;
    if (livesLeft < 0) {
      isPlaying = false;
      gameIsReadytoPlay = false;
      isNewHighScore = checkHighScore();
    }
    else {
      isImortal = true;
      imortalityTimer = millis();
    }
  }
}

// display stuf on the matrix
void displayGame() {
  lc.clearDisplay(0);
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
  for (int i = 0; i <= matrixSize; i++) {
    if (enemyColumns[i] > 0) {
      lc.setLed(0, enemyColumns[i] - 2, i, true);
      lc.setLed(0, enemyColumns[i] - 3, i, true);
    }
    if (shootColumns[i] > 0) {
      lc.setLed(0, shootColumns[i], i, true);
    }
  }
  if (isPlaying == false) {
    lc.clearDisplay(0);
  }
}

// game logic

void game() {

  if (millis() - timer > frame) {
    //
    currentScore += scoreByDificulty;
    needsLCDUpdate = true;
    timer = millis();
    if (frameCounter == increaseSpeed) {
      frame = frame / speedConstant;
      frameCounter = 0;
    }
    randomInt = random(0, matrixSize + 1);
    // if no obj in selected column create one object in row 1
    if (enemyColumns[randomInt] == 0) {
      enemyColumns[randomInt] = 1;
    }
    frameCounter++;


    for (int i = 0; i <= matrixSize; i++) {
      if (enemyColumns[i] == 10) { // object out of view
        enemyColumns[i] = 0;
        checkLivesLeft();
      }
      if (enemyColumns[i] != 0) {
        enemyColumns[i]++;
      }
    }

    needScreenUpdate = true;
  }
  if (millis() - timerBullet > bulletSpeed) {
    for (int i = 0; i <= matrixSize; i++) {
      if (shootColumns[i] != 0) {
        if (shootColumns[i] < enemyColumns[i]) {
          shootColumns[i] = 0;
          enemyColumns[i] = 0;
        } else {
          shootColumns[i]--;
        }
      }
    }
    timerBullet = millis();
    needScreenUpdate = true;
  }
  if (enemyColumns[ship] == 10 or enemyColumns[ship] == 9) {
    checkLivesLeft();
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
}
// function for shooting at the press of the button
void shoot() {
  volatile unsigned long intrerruptTime = 0;
  intrerruptTime = millis();
  if (intrerruptTime - lastPress > debounceTime) {
    if (shootColumns[ship] == 0) {
      shootColumns[ship] = 7;
      tone(buzzerPin, 800, 200);
    }

  }
  lastPress = millis();
}

// joystick button function
void buttonIsPressed() {
  volatile unsigned long intrerruptTime = 0;
  intrerruptTime = millis();
  if (intrerruptTime - lastPress > debounceTime) {
    currentMenu().onPress();
    needsLCDUpdate = true;
  }
  lastPress = millis();
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
  debounceButton();
  // control the menu with joystick
  jsForMenu();
  // exception for about menu that always needs to be updated
  // if the screen needs to be update it's going to be updated
  if (needsLCDUpdate == true || currentState == 4) {
    needsLCDUpdate = false;
    currentMenu().displayMenu(lcd);

  }

  // initialize the game if the game is over
  if (isPlaying == false) {
    gameInitialization();
  }
  // game is running
  if (isPlaying == true) {

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
