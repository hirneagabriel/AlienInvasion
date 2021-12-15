
## Small introduction
### AlienInvasion is a project made by me during the course Introduction to Robotics. In this repository I'm going to write the progress of my project and details of the game.
<br>
<details>
<summary> __STORY OF THE GAME__
  </summary>

#### AlienInvasion is a game where you are the last remaining hope of humankind! Get in your ship and defend the earth from the aliens! Kill as much enemys as you can. Don't let enemies pass you or the Earth is doomed!!!
  
</details>

<details>
<summary>__HOW DOES THE GAME WORK__</summary>

#### The game is quite simple you are a ship, and you try to shoot as many enemies as you can without letting them pass us. So how do I represent that?
  
- ##### In an 8x8 led matrix the ship is... a dot and the enemies are... also red dots. But I will make the enemies 2 dots so we can differentiate the enemy from the player.
  
- ##### For the controls, I choose a joystick. And for the shooting part, I'm going to use a separate button. Oh, and the thingy that we shoot is going to be? Yes! You guessed it a dot!
  
- ##### So we have the ship that is going to move left and right on the last row. And we have the enemies coming toward us on the columns at a speed that is going to increase over time. If the enemies pass us or hit us, we are going to lose a life. If we lose all lives we die.
</details>

<details>
<summary> __SETUP__
  </summary>

#### Used components:
  - ##### Arduino Uno
  - ##### one joystick
  - ##### one button
  - ##### one buzzer
  - ##### one 8x8 led matrix
  - ##### one 2x16 LCD Display
  - ##### one MAX7219 Driver
  - ##### 10k resitor 100 resitor 220 resitor and 1k resistor
  - ##### 10 µF capacitor and some ceramic capacitor
  - ##### some breadboards(I used 3)
 #### I'm not going to show how to connect each component because I have already connected all the components. I'm also too lazy to do a complete tutorial for this stuff. But I'm going to put some useful links(in the future).
 #### Version 1.0. It was my first time putting the project altogether and it worked but it had some imperfect contacts.
  
  ![20211208_131605](https://user-images.githubusercontent.com/61494964/145199783-2cd0015d-64a4-4c7f-849d-834ab1ea010d.jpg)
 #### Good luck with connecting all that wires! I mean, at least you don't have to go with this setup to the faculty and show it to the teacher in a WORKING STATE!  
 #### After a 4 hour try to solder the project on a circuit board, I give up on the soldering thing because of the lack of time. 
 #### So I build Version 1.2!
 ##### Front view
  ![20211215_124230](https://user-images.githubusercontent.com/61494964/146172343-776d8501-8762-4187-b89f-199f9d07fb94.jpg)
 ##### Back view
  ![20211215_124258](https://user-images.githubusercontent.com/61494964/146172442-86fddb9f-018a-460e-8b45-6d527258aafe.jpg)

 #### It's a more stable setup that is fixated better in the cardboard box. I've never had a problem with the wire connection so far, and I can transport it safely to faculty in my bag. Probably the final state. Maybe in the future, I will make a more good looking setup in my free time. 
 
</details>

<details>
<summary> __THE CODE__
  </summary>

#### The code can be found <a href="https://github.com/hirneagabriel/AlienInvasion/blob/main/AlienInvasion.ino" target="_blank" rel="noreferrer noopener">here</a>. 
#### I'll explain the important functions that are not that straight forward. 
#### For the menu part, I created an abstract class with some abstract functions. The onUp() onDown() onLeft() onRight() onPress() functions are called by the joystick menu function. And the displayMenu() function is being called when the LCD screen needs updates. Mostly when the joystick is used.
```C++
class Menu {
  public:
    virtual void onLeft() {};
    virtual void onRight() {};
    virtual void onUp() {};
    virtual void onDown() {};
    virtual void onPress() {};
    virtual void displayMenu(LiquidCrystal& lcd) = 0;
};
```
#### Each menu has its class that inheritance the abstract class.
#### I'm going to explain a little bit class Settings Menu. the other classes have similar functionality. 
```C++
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
```
- #### menu is an array of Strings. each string is an option
- #### pointingArrow is the variable that indicates an option. if poitingArrow == 0 than the option is Username
- #### bools that indicate if an option is selected
```C++
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
```
- #### called when an option is modified and saves the data to EEPROM.
```C++
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
```
- #### called at the press of the joystick and change the current option to selected or not selected
</details>
