
## Small introduction
### AlienInvasion is a project made by me during the course Introduction to Robotics. In this repository I'm going to write the progress of my project and details of the game.
<br>
<details>
<summary> __STORY OF THE GAME__
  </summary>

#### AlienInvasion is a game where you are the last remaining hope of humankind! Get in your ship and defend the earth from the aliens! Kill as much enemys as you can. Don't let enemies pass you or the Earth is doomed!!!
#### The game is inspired by old games like <a href="https://www.youtube.com/watch?v=tKobl50jrLk" target="_blank" rel="noreferrer noopener">Space Impact</a> and <a href="https://www.youtube.com/watch?v=MU4psw3ccUI&t=99s" target="_blank" rel="noreferrer noopener">Space Invaders</a>. 
  
</details>

<details>
<summary>__HOW DOES THE GAME WORK__</summary>

#### The game is quite simple you are a ship, and you try to shoot as many enemies as you can without letting them pass us. So how do I represent that?
  
- ##### In an 8x8 led matrix the ship is... a dot and the enemies are... also red dots. But I will make the enemies 2 dots so we can differentiate the enemy from the player.
  
- ##### For the controls, I choose a joystick. And for the shooting part, I'm going to use a separate button. Oh, and the thingy that we shoot is going to be? Yes! You guessed it a dot!
- ##### If the joystick is pressed a special ability will take place (a laser will kill all the enemies). 
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
  - ##### 2 10 µF capacitor and one ceramic capacitor
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
#### The project is built primarily in 2 parts:
##### - LCD menu
##### For the menu, I created an abstract class that will serve as a base for each menu option(Play, HiScore, Settings, About) with methods that will be called by the joystick commands. From that abstract class, I created other classes that will inherit the abstract class. I did that so I could control all the menus with one function. 
##### - Game logic
##### The game has 2 main functions: one for game logic and one for displaying on the matrix.
##### The game function controls all the logic behind the game. Each game object can only move on one column. So for each type of object, I made an array of length 8. One controls the enemies, one the bullets, and one the laser drop.  So if the arrayOfBullet[1] = 0 that means that there are no bullets in column 1, if the arrayOfBullet[1] = 6 that means that there is one bullet in collum 1 row 6.  
##### The enemies will be spawned randomly. They will stay still for one frame duration, then descend to the player. Depending on the difficulty, at k frames, the frame duration will decrease. If one enemy is out of view on a matrix the enemy will be set to 0 and one life from the player will be taken. If a collision with the ship is detected also one life is taken. If there are no lives left the game is over and the score is compared to the leaderboard. 
#### A video demo can be found <a href="https://www.youtube.com/watch?v=IJ5TDT52_ro" target="_blank" rel="noreferrer noopener">here</a>.
</details>
