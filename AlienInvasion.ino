#include<LiquidCrystal.h>
#include "LedControl.h" //  need the library
#include <EEPROM.h>

const int RS = 13;
const int enable = 10;
const int d4 = A2;
const int d5 = A3;
const int d6 = A4;
const int d7 = A5;
const int contrastPin = 5;
const int ledDisplayPin = 6;

const int VRx = A0;
const int VRy = A1;
const int SW = 2;

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 7;
const int rows = 8;
const int cols = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

const int minThreshold = 300;
const int maxThreshold = 600;
int xValue = 0;
int yValue = 0;
bool isMovedX = false;
bool isMovedY = false;


LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

const int shootButton = 3;


volatile unsigned long lastPress = 0;
const int debounceTime = 150;
int currentState = 0;
int currentScore = 0;
const int nameLenght = 3;
const int maxLevel = 4;
int livesLeft = 3;

bool firstSave = false;
int eeAddress = 0;
//data stored in eeprom
char* scoreNames[4] = {
  "AAA", "AAA", "AAA"
};

int scores[] = {
  0, 0, 0
};
int currentBrightness = 100;
int currentContrast = 130;
int matrixIntensity = 2;
char username[nameLenght + 1] = "AAA";
int activeLevel = 1;


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

//game global values
int ship = 0;
int matrixSize = 7;
long int lastMoved = 0;
int  moveInterval = 100;
bool isPlaying = false;
bool gameIsReadytoPlay = true;
bool isNewHighScore = false;
class Menu {
  public:
    virtual void onLeft() {};
    virtual void onRight() {};
    virtual void onUp() {};
    virtual void onDown() {};
    virtual void onPress() {};
    virtual void onPlay() {};
    virtual void displayMenu(LiquidCrystal& lcd) = 0;
};

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
          onPlay();
        }
      }
    }
    void onPlay() {
      if (isPlaying == true) {
        //
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
        lcd.print("Lives: ");
        lcd.print(livesLeft);
        lcd.print(" lvl: ");
        lcd.print(activeLevel);

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
            lcd.print("New HiScore!");
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

class SettingsMenu : public Menu {
    int blinkDelay = 500;
    String menu[5] = {
      "Username", "Level", "Contrast", "LCD Bright", "M Bright"
    };
    bool isEditing = false;
    bool editName = false;
    bool editLevel = false;
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
      EEPROM.put(eeAddress, activeLevel);
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
        editLevel = !editLevel;
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
      if (editLevel == true) {
        activeLevel -= 1;
        if (activeLevel <= 0) {
          activeLevel = 4;
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
      if (editLevel == true) {
        activeLevel += 1;
        if (activeLevel > maxLevel) {
          activeLevel = 1;
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

    void displayMenu(LiquidCrystal& lcd) {
      lcd.clear();
      if (pointingArrow < 4) {
        lcd.print(" ");
        lcd.print(menu[pointingArrow]);
        lcd.print(" ");
        if (pointingArrow == 0)
          lcd.print(username);
        if (pointingArrow == 1)
          lcd.print(activeLevel);
        if (pointingArrow == 2)
          lcd.print(currentContrast);
        if (pointingArrow == 3)
          lcd.print(currentBrightness);
        lcd.setCursor(0, 1);
        lcd.print(" ");
        lcd.print(menu[pointingArrow + 1]);
        lcd.print(" ");
        if (pointingArrow == 0)
          lcd.print(activeLevel);
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
    }
};
class AboutMenu : public Menu {
  long int timerScroll = 0;
  int intervalLeft = 0;
  int intervalRight = 16; 
  String message = "Made by Hirnea Gabriel. Github link: https://github.com/hirneagabriel/AlienInvasion";
  public:
    
    void onLeft() {
      currentState = 0;
    }

    String scrollText(String text){
      String result;
      String proccesString = "               " + text + "             ";
      result = proccesString.substring(intervalRight,intervalLeft);
      intervalLeft++;
      intervalRight++;
      if(intervalRight>proccesString.length()){
        intervalRight = 16;
        intervalLeft = 0;
      }
    return result;
    }
    void displayMenu(LiquidCrystal& lcd) {
      
      if(millis()-timerScroll > 400){
        lcd.clear();
        timerScroll = millis();
        lcd.print(" Alien Invasion");
        lcd.setCursor(0,1);
        lcd.print(scrollText(message));
        timerScroll = millis();
      }
    }


};

MainMenu mainMenu;
HiScoreMenu hiScoreMenu;
SettingsMenu settingsMenu;
PlayMenu playMenu;
AboutMenu aboutMenu;

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


void jsForMenu() {
  xValue = analogRead(VRx);
  yValue = analogRead(VRy);

  if (yValue <= minThreshold && isMovedY == false) {
    isMovedY = true;
    currentMenu().onDown();
  }

  if (yValue >= maxThreshold && isMovedY == false) {
    isMovedY = true;
    currentMenu().onUp();
  }

  if (xValue <= minThreshold && isMovedX == false) {
    isMovedX = true;
    currentMenu().onRight();
  }

  if (xValue >= maxThreshold && isMovedX == false) {
    isMovedX = true;
    currentMenu().onLeft();
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
    isMovedX = false;
  }

  if (yValue >= minThreshold && yValue <= maxThreshold) {
    isMovedY = false;
  }
}

void getData() {
  int address = 0;
  EEPROM.get(address, firstSave);
  address += sizeof(bool);
  if (firstSave == true) {
    EEPROM.get(address, username);
    address += sizeof(username);
    EEPROM.get(address, activeLevel);
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

void putData() {
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

const int blinkDelay = 100;
bool isImortal = true;
const int imortalityDuration = 3000;
long int imortalityTimer = 0;
bool needScreenUpdate = false;
long int timer = 0;
int frame = 1000;
int frameCounter = 0;
const int increaseSpeed = 3;
const float speedConstant = 1.02;
int randomInt;
int columns[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
volatile int shootColumns[] = {
  0, 0, 0, 0, 0, 0, 0, 0
};
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
bool checkHighScore() {
  for (int i = 0 ; i < 3; i++) {
    if (scores[i] < currentScore) {
      for (int j = 2 ; j >= i ; j--) {
        scores[j] = scores[j - 1];
        scoreNames[j] = scoreNames[j - 1];
      }
      scores[i] = currentScore;
      scoreNames[i] = username;
      putData();
      return true;
      break;
    }
  }
  return false;
}
void checkLivesLeft() {
  if (isImortal == false) {
    livesLeft--;
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
void displayGame() {
  //display ship
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
    if (columns[i] > 0)
      lc.setLed(0, columns[i] - 2, i, true);
    lc.setLed(0, columns[i] - 3, i, true);
    if (shootColumns[i] > 0)
      lc.setLed(0, shootColumns[i], i, true);
  }
  if (isPlaying == false) {
    lc.clearDisplay(0);
  }
}

void game() {
  if (millis() - timer > frame) {
    //score how many ticks
    currentScore++;
    timer = millis();
    if (frameCounter == increaseSpeed) {
      frame = frame / speedConstant;
      frameCounter = 0;
    }
    randomInt = random(0, matrixSize + 1);
    // if no obj in selected column create one object in row 1
    if (columns[randomInt] == 0) {
      columns[randomInt] = 1;
    }
    frameCounter++;


    for (int i = 0; i <= matrixSize; i++) {
      if (columns[i] == 10) { // object out of view
        columns[i] = 0;
        checkLivesLeft();
      }
      if (columns[i] != 0) {
        columns[i]++;
      }
    }

    needScreenUpdate = true;
  }
  for (int i = 0; i <= matrixSize; i++) {
    if (shootColumns[i] != 0) {
      if (shootColumns[i] < columns[i]) {
        shootColumns[i] = 0;
        columns[i] = 0;
      } else {
        shootColumns[i]--;
      }
    }
  }
  needScreenUpdate = true;

  if (columns[ship] == 10 or columns[ship] == 9) {
    checkLivesLeft();
    //isPlaying = false;
    //Serial.println("Collision");
  }
  if (isPlaying == false) {
    for (int i = 0; i <= matrixSize; i++) {
      columns[i] = 0;
      shootColumns[i] = 0;
    }
  }

}


void shoot() {
  volatile unsigned long intrerruptTime = 0;
  intrerruptTime = millis();
  if (intrerruptTime - lastPress > debounceTime) {
    if (shootColumns[ship] == 0)
      shootColumns[ship] = 7;

  }
  lastPress = millis();
}
void buttonIsPressed() {
  volatile unsigned long intrerruptTime = 0;
  intrerruptTime = millis();
  if (intrerruptTime - lastPress > debounceTime) {
    currentMenu().onPress();

  }
  lastPress = millis();
}




void setup() {

  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  //putData();
  getData();

  lcd.begin(16, 2);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(contrastPin, OUTPUT);
  pinMode(ledDisplayPin, OUTPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(shootButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SW), buttonIsPressed, FALLING);
  attachInterrupt(digitalPinToInterrupt(shootButton), shoot, FALLING);
  analogWrite(contrastPin, currentContrast);
  analogWrite(ledDisplayPin, currentBrightness);
  lcd.createChar(0, arrow);
  lcd.createChar(1, arrowUp);
  lcd.createChar(2, arrowDown);
  lcd.clear();
  Serial.begin(9600);
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixIntensity);
  lc.clearDisplay(0);
  lcd.print(" AlienInvasion");
  delay(3000);

}

void loop() {

  jsForMenu();
  currentMenu().onPlay();
  currentMenu().displayMenu(lcd);
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
  delay(30);
  for (int i = 0; i < 3 ; i++) {
    Serial.print(scores[i]);
  }
}
