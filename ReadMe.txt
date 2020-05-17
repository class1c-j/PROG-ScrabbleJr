=========================================================================
#                                                                       #
#  FEUP - MIEIC - PROG - 2019/2020                                      #
#  Projeto 2 - Scrabble Junior                                          #
#                                                                       #
#  TURMA 6 - GRUPO 5                                                    #
#  Adelaide Miranda Santos (up201907487)                                # 
#  Rita Miguel Matos Mendes (up201907877)                               #
#                                                                       #
=========================================================================

  (1) - Project description

This project was developed for the 1st Year - 2nd Semester MIEIC course
of Programming and it includes 2 programmes - BoardBuilder and ScrabbleJr.

  (2) - Project objectives (adapted from the assignment statement)

The main objectives of this work were practincing creating a project
following the "object oriented" programming paradigm, selecting data
structures and algorithms suitable for the implementation of the game
and taking advantage of the data structures and algorithms available
in the C++ STL. Other objectives like the use of files and thedevelopment
of simple and robust program interfaces are also present.

  (3) ?

=========================================================================
|| BOARD BUILDER                                                       ||
=========================================================================
  (1) - Pruposed objectives and development state
=========================================================================

  -> The programme must allow to select the size of the game board:
  
  ---- ACHIEVED: the programme prompts the user to choose the desired
  number of lines and the desired number of columns, validating it
  according to the maximum of 20x20.


  -> The programme must allow to create the game board, selecting a set of
  words, from the words available in a dictionary provided in the WORDS.TXT
  file, and positioning those words on the board, so that they intersect
  correctly. The position of the words must be specified by the user,
  indicating the of the first letter of the word and it's orientation,
   horizontal or vertical. It is up to the program to verify that the
   words are valid, that they can be placed in the selected position and
   that they intersect correctly.
   
   ---- ACHIEVED: the programme asks the user to insert the word, the
   starting line and the starting column, validating all of them. After
   that, the programme checks if the insertion causes a problematic
   situation and show an error in that case, not allowing for the user
   to create invalid boards.


   -> The programme must allow to save the contents of the board to a
   text file

   ---- ACHIEVED: When the player chooses to save the board, the programme
   saves all of its words and respective information on a text file
   (that also includes a representation of the board)


   -> This programme mus be implemented using at least one classe defined
   by the programmer: class Board, to represent a board.

   ---- ACHIEVED
=========================================================================
   (2) - Added features
=========================================================================

   -> A menu was implemented to make it easier for the user to interact
   with the programme.

   -> The ability to edit an existing board file was added.
   
   -> The ability to remove a word already inserted in the board was added.

=========================================================================
   (3) - Known issues (to do)
=========================================================================

=========================================================================
   (4) - Remarks
=========================================================================

Due to the lack of clear specification of the conditions of
intersection of words in the project specification, we considered that:

	-> words with the same orientation cannot be next to each other:

	   - horizontal words cannot be inserted if there is already a
	   word in the previous or next lines in the word's width range.
	   - vertical words cannot be inserted if there is already a word
	   in the previous or next columns in the word's width range.

	-> when inserting a word a new larger word is allowed to be formed,
	either vertically or horizontally if it is valid.


=========================================================================
|| SCRABBLE JR                                                         ||
=========================================================================
  (1) - Pruposed objectives and development state
=========================================================================

  -> This program must allow 2 to 4 players to play the game, detect the
  end of the game and announce the winner.

  ---- ACHIEVED (ADAPTED): The game allows for 1 to 4 players (number
  chosen in the start of the game), checks if the board is all played and
  in the end shows the score of all players, sorted.


  -> The program must be implemented using some "classes" defined by the
  programmer, for example, to represent the board (Board), a player
  (Player), and the "bag" of letters (Pool); others may be useful or
  convenient.
  Note: taking into account the possibility of building your own boards,
  using the Board Builder program, the number of letter tiles may be
  different from those specified in reference; the letter tiles in the
  pool must be the ones necessary to build the words in the board.

  ---- ACHIEVED: All the suggested classes were implemented. The pool is
  generated according to all the letters that are in each board.


  -> The code must be structured in order to separate the definition of
  each class and the implementation of its methods, in files with the
  extension .hpp(or .h) and .cpp, respectively.

  ---- ACHIEVED


  -> The game board must be read from a text file, previously created.
  See the specification of the "Board Builder" program for the format of
  that file.

  ---- ACHIEVED


  -> The number of players and the name of the board file must be asked to
  the user at the beginning of the program.

  ---- ACHIEVED: The user is prompted to choose those when they start a new game


  -> The game board must be shown with an aspect similar to that illustrated
  in figure 2. To specify the position of the letter to be played, users must
  use an uppercase letter to specify the line and a lowercase letter to
  specify the column. The range of the letters depends on the size of the board).

  ---- ACHIEVED


  -> The program must prevent the playing of invalid letters (letters that
  are not in the player's hand) or at invalid positions(the played letter
  does not match with the letter in the chosen position).

  ---- ACHIEVED: The game will show the adequate error messages and will not
  allow the user to preform those moves.


  -> The way to indicate that a letter has been played on the board is to
  change the color of the letter on the screen.

  ---- ACHIEVED: When a tile is placed, that place in the board will become
  highlighted in "engineering colour".
=========================================================================
   (2) - Added features
=========================================================================

  -> An hint giving feature was implemented. When prompted to play, the
  user may type "hint" and the game will suggest a valid place where they
  can play.

  -> The menu was also used here, though only at the begining.

  -> The game's interface will adapt itself according to the size of the
  board chosen
  
=========================================================================
   (3) - Known issues (to do)
=========================================================================

  -> When the player inserts an invalid input for the "insert letter"
  prompt, the error message will appear in a bad position, due to
  the board not being passed to the function (will fix probably)

========================================================================
