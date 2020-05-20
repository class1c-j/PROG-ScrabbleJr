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

This project was developed for the 1st Year 2nd Semester MIEIC course
of Programming and it includes 2 programs - BoardBuilder and ScrabbleJr.

  (2) - Project objectives (adapted from the assignment statement)

The main objectives of this work were practicing creating a project
following the "object oriented" programming paradigm, selecting data
structures and algorithms suitable for the implementation of the game
and taking advantage of the data structures and algorithms available
in the C++ STL. Other objectives like the use of files and the development
of simple and robust program interfaces are also present.

   (3) - Information on how to run the programs correctly
   
To run a program, you should compile it's specific files as well as the 
files placed in the CommonFiles directory. For example, to compile
ScrabbleJr using g++ from the ScrabbleJr directory, the command would
look like g++ ./*.cpp ./../CommonFiles/*.cpp.

The paths to the boards are hardcoded, so in order to detect the boards
correctly, please keep the project structure as it is and the program 
should be run from it's directory.

=========================================================================
|| BOARD BUILDER                                                       ||
=========================================================================

    a b c d e f g h i j k l m n o 
    ------------------------------
 A |  F           D     E G G S   
 B |  R   B     B U Z Z     O     
 C |M U S I C     C   E     A R M 
 D |  I   C       K   B O A T   O 
 E |  T O Y   S       R         N 
 F |      C   H       A R C A D E 
 G |      L   E   T       A     Y 
 H |    T E L E V I S I O N       
 I |          P   G       D O G   
 J |        J     E       Y   R   
 K |      W A T E R           A X 
 L |        C             B   P   
 M |    S   K         A   A   E   
 N |  Q U E E N     A P P L E S   
 O |    N   T         E   L       

This is edit mode. Choose from the options below to create your board.
(1) - Add new word
(2) - Erase existing word
(3) - Quit to main menu
(4) - Save current board


=========================================================================
   (1) - Proposed objectives and development state
=========================================================================

  -> We were able to achieve every objective that was proposed.
 
=========================================================================
   (2) - Added features
=========================================================================

  -> A menu was implemented to make it easier for the user to interact
   with the program.

  -> The ability to edit an existing board file was added.
   
  -> The ability to remove a word already inserted in the board was added.

=========================================================================
   (3) - Remarks and observations
=========================================================================

Due to the lack of clear specification of the conditions of
intersection of words in the project specification, we considered that:

  -> words with the same orientation cannot be next to each other:

    - horizontal words cannot be inserted if there is already a
    word in the previous or next lines in the word's width range.
 
    - vertical words cannot be inserted if there is already a word
    in the previous or next columns in the word's width range.

  -> when inserting a word, a new larger word is allowed to be formed,
  either vertically or horizontally if it is valid.


========================================================================
|| SCRABBLE JR                                                        ||
========================================================================


    a b c d e f g h i j k l m n o           mafarrico
    ------------------------------          Score: 0
 A |  F           D     E G G S             E H S S A G P 
 B |  R   B     B U Z Z     O     
 C |M U S I C     C   E     A R M           egas
 D |  I   C       K   B O A T   O           Score: 0
 E |  T O Y   S       R         N           O L S O A L C 
 F |      C   H       A R C A D E 
 G |      L   E   T       A     Y           becas
 H |    T E L E V I S I O N                 Score: 0
 I |          P   G       D O G             G E M R Z O A 
 J |        J     E       Y   R   
 K |      W A T E R           A X 
 L |        C             B   P             BOT Sonso will play 2 tiles. Press ENTER to continue
 M |    S   K         A   A   E   
 N |  Q U E E N     A P P L E S   
 O |    N   T         E   L       


 E R T C E O A 
 You (0 points)


=========================================================================
   (1) - Proposed objectives and development state
=========================================================================

  -> We were able to achieve every objective that was proposed.

=========================================================================
   (2) - Added features
=========================================================================

  -> An hint giving feature was implemented. When prompted to play, the
  user may type "hint" and the game will suggest a valid place where they
  can play.

  -> The menu was also used here, though only at the beginning.

  -> The game's interface will adapt itself according to the size of the
  board chosen

  -> The ability to play against a (very simple) bot was implemented.
  To do so, when prompted to write a player's name, write
  ".bot <name>".

========================================================================
   (3) - Known issues
========================================================================

   -> When the bots play, some fickering may be experienced on Windows.
========================================================================
