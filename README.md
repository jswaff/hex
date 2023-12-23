# hex

A simple hex playing program, done as part of a C++ refresher course.  As this is just a toy program, all the code is in a single file, alleviating the 
need for a Makefile.  Just do something like 'g++ hex.cpp' and then execute a.out.

The rules for Hex: https://www.maths.ed.ac.uk/~csangwin/hex/index.html


The interesting thing about this program is that it has zero knowledge of the game of hex, yet still manages a decent game by using a Monte Carlo approach.
When it is the computer's turn to play, it does something like this:

1) Get a list of all unoccupied squares.  These are the candidate moves.

2) For each candidate move, fill the remaining unoccupied positions at random, alternating colors.  (As if two idiots with no clue were playing.)  It doesn't
matter that the game may be over before the board is full - filling the remaining squares doesn't change the outcome of the game, and is faster/cheaper
then evaluating for a win after each move.

Do this many times, something like 10,000.  Record the win percentage for each candidate move.

3) Finally, pick the candidate move that had the highest win percentage.  Essentially, we are using that as a measure of the move's quality.  Note, draws
are impossible in Hex.  

That is not perfect by any means, but it does play surprisingly well for something with zero domain knowledge.

Below is a sample game played on a 5x5 board for illustration.  Note how the computer player played blocking moves!

```
Let's play a game of Hex!
Blue wins by connecting east to west.
Red wins by connecting north to south.

What size board?  Enter a number 5-11.
5
Which color would you like - blue or red? (blue goes first)
Enter your choice (b/r):
b
Blue it is!

. - . - . - . - .
 \ / \ / \ / \ / \
  . - . - . - . - .
   \ / \ / \ / \ / \
    . - . - . - . - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
3 3
The computer chooses move 3 4
win % after 10000 simulations: 44.95%

. - . - . - . - .
 \ / \ / \ / \ / \
  . - . - . - . - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
2 3
The computer chooses move 2 4
win % after 10000 simulations: 56.07%

. - . - . - . - .
 \ / \ / \ / \ / \
  . - . - B - R - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
1 3
The computer chooses move 1 4
win % after 10000 simulations: 72.49%

. - . - B - R - .
 \ / \ / \ / \ / \
  . - . - B - R - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
4 4
The computer chooses move 4 3
win % after 10000 simulations: 86.05%

. - . - B - R - .
 \ / \ / \ / \ / \
  . - . - B - R - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - R - B - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
5 3
The computer chooses move 5 2
win % after 10000 simulations: 100%

. - . - B - R - .
 \ / \ / \ / \ / \
  . - . - B - R - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - R - B - .
       \ / \ / \ / \ / \
        . - R - B - . - .

BLUE to move

Game over.  The winner is ... RED!Let's play a game of Hex!
Blue wins by connecting east to west.
Red wins by connecting north to south.

What size board?  Enter a number 5-11.
5
Which color would you like - blue or red? (blue goes first)
Enter your choice (b/r):
b
Blue it is!

. - . - . - . - .
 \ / \ / \ / \ / \
  . - . - . - . - .
   \ / \ / \ / \ / \
    . - . - . - . - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
3 3
The computer chooses move 3 4
win % after 10000 simulations: 44.95%

. - . - . - . - .
 \ / \ / \ / \ / \
  . - . - . - . - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
2 3
The computer chooses move 2 4
win % after 10000 simulations: 56.07%

. - . - . - . - .
 \ / \ / \ / \ / \
  . - . - B - R - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
1 3
The computer chooses move 1 4
win % after 10000 simulations: 72.49%

. - . - B - R - .
 \ / \ / \ / \ / \
  . - . - B - R - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
4 4
The computer chooses move 4 3
win % after 10000 simulations: 86.05%

. - . - B - R - .
 \ / \ / \ / \ / \
  . - . - B - R - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - R - B - .
       \ / \ / \ / \ / \
        . - . - . - . - .

BLUE to move

Enter your move (row col):
Note: the top left corner is 1 1
5 3
The computer chooses move 5 2
win % after 10000 simulations: 100%

. - . - B - R - .
 \ / \ / \ / \ / \
  . - . - B - R - .
   \ / \ / \ / \ / \
    . - . - B - R - .
     \ / \ / \ / \ / \
      . - . - R - B - .
       \ / \ / \ / \ / \
        . - R - B - . - .

BLUE to move

Game over.  The winner is ... RED!
```





