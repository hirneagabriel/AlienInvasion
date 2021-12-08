
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
  - ##### 10k resitor 100 resitor and 220 resitor
  - ##### 10 µF capacitor and some ceramic capacitor
  - ##### some breadboards(I used 3)
 #### I'm not going to show how to connect each component because I have already connected all the components. I'm also too lazy to do a complete tutorial for this stuff. But I'm going to put some useful links(in the future).
 #### And if you think that my excuse for being lazy isn't good. Well, behold!
  
  ![20211208_131605](https://user-images.githubusercontent.com/61494964/145199783-2cd0015d-64a4-4c7f-849d-834ab1ea010d.jpg)
 #### Good luck with connecting all that wires! I mean, at least you don't have to go with this setup to the faculty and show it to the teacher in a WORKING STATE!  
</details>

<details>
<summary> __THE CODE__
  </summary>

#### Don't worry about it. Just copy paste <a href="https://github.com/hirneagabriel/AlienInvasion/blob/main/AlienInvasion.ino" target="_blank" rel="noreferrer noopener">this</a>. (kidding I'm going to explain it but not right now).
  
</details>
