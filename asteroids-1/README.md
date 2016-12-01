## Computer Architecture Project 3: MSP430 Game with LED

This project was created by Jose Perez (josegperez@mail.com) and Diego Reynoso (dareynoso@miners.utep.edu). Special thanks to all the resources cited in the code and the template code provided by our TAs and professor.

This project was created for the TI MSP430 with a custom EduKit BoosterPack that contains 4 buttons, a buzzer, and an LCD screen.

This project contains the following files:
* astroids.c:

 Is the main file where the game is played and uses if statmenets for the different phases like Playing and Gameover Screen

* astroids.h:

 Initializes the main methods we will use

*astroids_shapes.h:

Is where all the shapes are created.


* buzzer.h:

 Allows other components to modify the internal state of the buzzer.

* buzzer.c:

 We tried to implement a song to play once you win the game but we ran out of memory and were not able to implement it.

* led.c is where the led is used when the cpu is used.

* timer.h:

 Allows other components to change the state of the timer and access to the pseudo-random number generator.

* timer.c:

 Controls the timer to be able to create the Score and the program as well as an implementation of a pseudo-random number generator to be able to show the stars and also most of the points for the score.

*mov_layer.c:
is where the layers are able to move

rock.c
This is where the stars in the back are created they are created randomly

*shapemotion.c
is where the main parts of the playing field are like the boarder or the String to show the Lives you have left.

*star.c
is where we create stars

*str_util.c
Is where we do the utils

When the program is first loaded it shows the boss a big square and the triangle ship which is the ship you control.What you have to do is to shoot a tiny square into the boss to take some heath away. The longer you play the more points you can get, you only have 5 lives to beat him:

The buttons we use in the game are:

S2 is used so that you can rotate the ship and also move the button a little bit.

S3 is used to propel the ship in any direction it has

S4 is used shoot the gun.

To compile:
~~~
$ make
~~~

To install in the MSP430 (requires it to be connected to the computer):
~~~
$ make load
~~~

To delete binaries:
~~~
$ make clean
~~~