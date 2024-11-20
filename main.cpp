#include <iostream>
#include <string>
#include "src/mineSweeper/mineSweeper.cpp"
#include "src/cinParse/cinParse.cpp"
using namespace std;

void printInstructions();
void printBoard(mineSweeper &);
void printInvalidInput(string error = "");
/*
  amir gorkovchenko - (AG)
*/

/*
  if-else/switch
    (file: "main.cpp") lines: 79, 82, 100, 113, 129, 171, 185, 189... 207
    (file: "mineSweepers.cpp") lines: 7, 11, 38, 46... 394
    (file: "cinParse.cpp") lines: 13, 23, 25, 54... 154... 211

  loops
    (file: "main.cpp") lines: 76, 94, 197, 273, 287, 293, 301, 309, 320, 330, 402, 408
    (file: "mineSweepers.cpp") lines: 62, 151, 255
    (file: "cinParse.cpp") lines: 17, 68, 88, 140, 190

  functions
    (file: "main.cpp") lines: 66, 247, 267, 423
    (file: "mineSweepers.cpp") lines: 4, 32, 134, 181, 203, 209, 215, 229... 
    (file: "cinParse.cpp") lines: 4, 242, 247, 251, 259, 272

  array/vector
    (file: "mineSweepers.h") lines: 134

  struct
    (file: "mineSweepers.h") lines: 22
    (file: "cinParse.h") lines: 13, 18

  OOP
    (file: "main.cpp") lines: 69, 92, 165
    (file: "mineSweepers.h") lines: 12
    (file: "cinParse.h") lines: 9

  pointers
    (file: "mineSweepers.h") lines: 136
    (file: "mineSweepers.cpp") lines: 35, 128, 140, 174, 184

  stringstream/external file
    (file: "mineSweepers.cpp") lines: 35, 140, 184

  recursion
    (file: "mineSweepers.cpp") lines: 346

  exceptions
    (file: "cinParse.cpp") lines: 61, 81, 133, 183
  
   - (AG)
*/

/**
 * input: take cin input and proccess accordingly
 * proccess: handle minesweeper game engine,
 *            react to cin-inputs and play game according to inputs.
 *            exit game on input command
 * output: cout, game graphics using ascii characters
 *  - (AG)
 */
int main()
{
  // initialize game object and run flag - (AG)
  mineSweeper game;
  bool run = true;

  // print initial instructions: - (AG)
  printInstructions();

  // main game loop: - (AG)
  while (run)
  {
    // run program differently based on current gam state: - (AG)
    if (game.getStatus() != 2)
    {
      // no game started, check if saved game can be found. print accordingly - (AG)
      if (game.checkSavedGame())
      {
        // It looks like you have a saved game, to continue, press ‘c’. To start a new game, press ‘s’ - (AG)
        cout << "It looks like you have a saved game, to continue, press 'c'. ";
      } // end if
      // ...To start a new game, press 's' - (AG)
      cout << "To start a new game, press 's'..." << endl;

      // prompt for input continually until valid input: - (AG)
      bool validInput = false;
      cinParse parseInput; // input parse object - (AG)

      while (!validInput)
      {
        // this will wait until any input: - (AG)
        parseInput.parse();

        // could have used a switch, but not changing anything major without the chance to test on school computers - (AG)
        if (parseInput.getCharCommand() == 1 /*"s"(start game) - (AG)*/)
        {
          // setup default board size here: - (AG)
          const int default_x = 10;
          const int default_y = 10;

          game.startNewGame(default_x, default_y); // defaults - (AG)
          cout << "Starting new game (" << to_string(default_x) << " x " << to_string(default_y) << ")" << endl;
          validInput = true; // flag and exit input prompt - (AG)
        }
        else if (parseInput.getCharCommand() == 2 /*"s x y"(start custom game) - (AG)*/)
        {
          // validate custom game size boarder:
          if (parseInput.getStartSize().coord_x >= 1 && parseInput.getStartSize().coord_x <= 26 &&
              parseInput.getStartSize().coord_y >= 1 && parseInput.getStartSize().coord_y <= 255)
          {
            // start custom game and display: - (AG)
            game.startNewGame(parseInput.getStartSize().coord_x, parseInput.getStartSize().coord_y);
            cout << "Starting new game (" << parseInput.getStartSize().coord_x << " x " << parseInput.getStartSize().coord_y << ")" << endl;
            validInput = true; // flag and exit input prompt - (AG)
          }
          else
          {
            // out of bounds, max allowed size is A-Z x 1-255 - (AG)
            printInvalidInput("Out of bounds. Max(26, 255)");
          } // end if
        }
        else if (parseInput.getCharCommand() == 3 /*"c"(continue saved game - (AG))*/)
        {
          if (game.startSavedGame()) // << check if saved game loads properly - (AG)
          {
            cout << "Game loaded" << endl;
            validInput = true; // game loaded. flag and exit input prompt - (AG)
          }
          else
          {
            cout << "Error loading game!" << endl;
          } // end if
        }
        else if (parseInput.getCharCommand() == 0 /*"x"(quit program) - (AG)*/)
        {
          // quit game:
          run = false;
          validInput = true;
        }
        else if (parseInput.getCharCommand() == 4 /*"?"(get help) - (AG)*/)
        {
          // help command - (AG)
          printInstructions();
        }
        else
        {
          // unrecognized command in this context - (AG)
          printInvalidInput();
        } // end if - (AG)
      }   // end while - (AG)
    }
    else
    {
      // game currently playing, new input context - (AG)
      printBoard(game);
      cout << "Enter Command..." << endl;

      // prompt for input continually until valid input: - (AG)
      bool validInput = false;
      cinParse parseInput;

      while (!validInput)
      {
        parseInput.parse();

        if (parseInput.getCharCommand() == 0 /*"x"(quit game, not program) - (AG)*/)
        {
          // save and quit current game: - (AG)
          cout << "Saving game..." << endl;
          game.saveGame();
          validInput = true;
        }
        else if (parseInput.getCharCommand() == 4 /*"?"(get help) - (AG)*/)
        {
          // help command - (AG)
          printInstructions();
        }
        else // if not char ^^^ command ^^^, then must be move vvv command vvv - (AG)
        {
          if (parseInput.isMoveCommand()) // check if valid move command was parsed - (AG)
          {
            // y is numerical, x is alpha numerical - (AG)
            // y starts from #1 - (AG)
            if (parseInput.getMoveCommand().coord_y >= 1 && parseInput.getMoveCommand().coord_y <= game.getSize_y() &&
                parseInput.getMoveCommand().coord_x >= 0 && parseInput.getMoveCommand().coord_x < game.getSize_x())
            {
              // check special cases, flagged blocks cant be dug up: - (AG)
              if (!parseInput.getMoveCommand().commandAction &&
                  game.getBlock(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1).isCovered &&
                  game.getBlock(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1).isFlagged)
              {
                // block is covered and flagged, cannot dig: - (AG)
                printInvalidInput("Block is flagged");
              }
              else
              {
                // set flag, run move according to given input: - (AG)
                validInput = true;
                game.move(parseInput.getMoveCommand().coord_x, parseInput.getMoveCommand().coord_y - 1, parseInput.getMoveCommand().commandAction);

                // check game run status after each move: - (AG)
                switch (game.getStatus())
                {
                case 0:
                  printBoard(game); // << print location of bombs - (AG)
                  cout << ">>>> Game Over! You Lost <<<<" << endl;
                  break;
                case 1:
                  printBoard(game); // << print location of bombs - (AG)
                  cout << ">>>> Congratulations! You Won <<<<" << endl;
                  break;
                } // end switch - (AG)
              }   // end if - (AG)
            }
            else
            {
              // out of bounds, print current bounds: - (AG)
              printInvalidInput(string("Out of bounds. Max(") + (char)(game.getSize_x() + 'a' - 1) + ", " + to_string(game.getSize_y()) + ")");
            } // end if - (AG)
          }
          else
          {
            // unrecognized command in this context - (AG)
            printInvalidInput();
          } // end if - (AG)
        }   // end if - (AG)
      }     // end while - (AG)
    }       // end if - (AG)
  }         // end while - (AG)

  // game exit: - (AG)
  cout << "Exiting..." << endl;
  return 0;
} // end main() - (AG)

/**
 * input: none
 * process: print simple game instructions
 * output: none
 *  - (AG)
 */
void printInstructions()
{
  cout << endl;
  cout << "To start a new game, press 's'. To start at custom size, type 's x y'" << endl;
  cout << "To save and exit anytime, press 'x'" << endl;
  cout << "Game Play:" << endl;
  cout << "  To enter a column, use letters a-h. To enter a row, use numbers 1-8" << endl;
  cout << "  To dig, use 'd' and to flag, use 'f'" << endl;
  cout << "  Type everything into a single command with row-column first then action like this: '1ad' or 'b3f'" << endl;
  cout << "To see these instructions again, press '?' anytime" << endl;
  cout << endl;
} // end printInstructions() - (AG)

/**
 * input: game object to be printed
 * process: determine state of game and each block
 *          print in easy to read format and react well to scaling(larger x and y axis)
 * output: none
 *  - (AG)
 */
void printBoard(mineSweeper &game)
{
  // calculate max blank space for formatting - (AG)
  uint8_t maxBlankSpace = 1;
  {
    int size_y_temp = game.getSize_y();
    while (size_y_temp /= 10)
    {
      maxBlankSpace++;
    } // end while - (AG)
  }   // end guard - (AG)

  int cnt_x;
  int cnt_y;

  // print alphabet indices - (AG)
  {
    // add some spaces before Alphabetical axis: - (AG)
    {
      uint8_t spaceIndex;
      for (spaceIndex = 0; spaceIndex <= maxBlankSpace; spaceIndex++)
      {
        cout << " ";
      } // end for - (AG)
    }   // end guard - (AG)
    // print alphabetical indices: - (AG)
    for (cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
    {
      cout << char('A' + cnt_x) << "  ";
    } // end for - (AG)
    cout << endl;
  } // end guard - (AG)

  //// iterate over rows(y axis block) //// - (AG)
  for (cnt_y = 0; cnt_y < game.getSize_y(); cnt_y++)
  {
    /**
     * calc digit length for each x axis number:
     *  - (AG)
     */
    uint8_t digitLen = 1;
    int cnt_y_temp = cnt_y + 1;
    while (cnt_y_temp /= 10)
    {
      digitLen++;
    } // end while - (AG)

    /**
     * add spaces before each x axis number:
     *  - (AG)
     */
    {
      uint8_t spaceIndex;
      for (spaceIndex = 0; spaceIndex < maxBlankSpace - digitLen; spaceIndex++)
      {
        cout << " ";
      } // end for - (AG)
    }   // end guard - (AG)

    // one more space before game row is printed - (AG)
    cout << cnt_y + 1 << ' ';

    // iterate over each x axis block - (AG)
    for (cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
    {
      // only reveal bombs on game-won or game-lost: - (AG)
      if (game.getStatus() == 0 || game.getStatus() == 1)
      {
        if (game.getBlock(cnt_x, cnt_y).isBomb)
        {
          /**
           * print 'O' for a won game
           * print 'X' for lost game:
           *  - (AG)
           */
          if (game.getStatus() == 1)
          {
            cout << "O ";
          }
          else
          {
            cout << "X  ";
          } // end if - (AG)
        }
        else
        {
          cout << "   ";
        } // end if - (AG)
      }
      else // normal game printing mode: - (AG)
      {
        if (game.getBlock(cnt_x, cnt_y).isCovered) // if block is covered - (AG)
        {
          // check if block is flagged: - (AG)
          if (game.getBlock(cnt_x, cnt_y).isFlagged)
          {
            cout << "O  "; // flagged block, 'O' seems to be easier to read - (AG)
          }
          else
          {
            cout << char(45) << "  "; // char(45) is a simple dash char, small and simple - (AG)
          }                           // end if - (AG)
        }
        else
        {
          /**
           * uncovered blocks can be one of 3 types
           *  bomb
           *  number hint
           *  empty
           *  - (AG)
           */
          if (game.getBlock(cnt_x, cnt_y).isBomb)
          {
            cout << "X  "; // bomb block, redundant - (AG)
          }
          else if (game.getBlock(cnt_x, cnt_y).blockNum > 0)
          {
            cout << to_string(game.getBlock(cnt_x, cnt_y).blockNum) << "  "; // number hint, convert to string and print - (AG)
          }
          else
          {
            cout << "   "; // blank block - (AG)
          }                // end if - (AG)
        }                  // end if - (AG)
      }                    // end if - (AG)
    }                      // end for - (AG)
    cout << endl;          // new line after each row - (AG)
  }                        // end for - (AG)

  // print alphabet indices - (AG)
  {
    // add some spaces before Alphabetical axis: - (AG)
    {
      uint8_t spaceIndex;
      for (spaceIndex = 0; spaceIndex <= maxBlankSpace; spaceIndex++)
      {
        cout << " ";
      } // end for - (AG)
    }   // end guard - (AG)
    // print alphabetical indices: - (AG)
    for (cnt_x = 0; cnt_x < game.getSize_x(); cnt_x++)
    {
      cout << char('A' + cnt_x) << "  ";
    } // end for - (AG)
  }   // end guard - (AG)
  cout << endl
       << endl;
} // end printBoard() - (AG)

/**
 * input: default arg, error string(for error clarification)
 * process: print invalid input message, if needed, input clarification message
 * output: none
 *  - (AG)
 */
void printInvalidInput(string error /*= "" - (AG)*/)
{
  cout << "Invalid Input! " << error << endl;
  cout << "Press '?' for help or try again" << endl;
} // end printInvalidInput() - (AG)
