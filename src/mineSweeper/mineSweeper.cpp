#include "mineSweeper.h"
using namespace std;

void mineSweeper::startNewGame(uint8_t size_x, uint8_t size_y)
{
  // Ensure that the minimum size for the game board is met - (AG)
  if (size_x < 1)
  {
    size_x = 1;
  } // end if - (AG)
  if (size_y < 1)
  {
    size_y = 1;
  } // end if - (AG)

  // Set the size of the game board - (AG)
  _boardSize_x = size_x;
  _boardSize_y = size_y;

  // Clear the existing game board and resize it to accommodate the specified size - (AG)
  _gameBoard.clear();
  _gameBoard.resize((int)_boardSize_x * (int)_boardSize_y); // make an empty board with default values - (AG)

  // setup the new board with bombs and hint numbers - (AG)
  _placeBombs();
  _placeNumbers();

  // Set the game status to game started - (AG)
  _gameStatus = 2;
} // end startNewGame() - (AG)

bool mineSweeper::startSavedGame()
{
  _gameStatus = -1; // set game to idle - (AG)
  ifstream game(_fileName);

  // Check if the file is open and can be read - (AG)
  if (game.is_open())
  {
    int size_x, size_y;

    // Read the size of the game board (size_x, size_y) from the file - (AG)
    game >> size_x >> size_y;

    // Check if the file has not reached the end yet - (AG)
    if (!game.eof())
    {
      // check board minimume's: - (AG)
      if (size_x >= 1 && size_y >= 1)
      {
        // Set the size of the game board - (AG)
        _boardSize_x = size_x;
        _boardSize_y = size_y;

        // Clear the existing game board and resize it to accommodate the specified size - (AG)
        _gameBoard.clear();
        _gameBoard.resize(_boardSize_x * _boardSize_y); // make an empty board with default values - (AG)

        bool doneParsing = false;

        // start parsing blocks from file: - (AG)
        while (!doneParsing && !game.eof() && !game.fail())
        {
          /**
           * test each starting value of each line
           * either index or sentinel  values '-1' indicating end of board setup
           *  - (AG)
           */
          int testVal;
          game >> testVal;

          // if the initial value in line is -1, start game and exit - (AG)
          if (testVal == -1 && !game.fail())
          {
            doneParsing = true;
            _gameStatus = 2;
          }
          // check for valid index: - (AG)
          else if (testVal >= 0 && testVal < _gameBoard.size())
          {
            int index, isCovered, isFlagged, isBomb, blockNum;
            index = testVal;

            // extract data in correct order: - (AG)
            game >> isCovered >> isFlagged >> isBomb >> blockNum;

            // set vector index struct accordingly: - (AG)
            _gameBoard[index].isCovered = (bool)isCovered;
            _gameBoard[index].isFlagged = (bool)isFlagged;
            _gameBoard[index].isBomb = (bool)isBomb;
            _gameBoard[index].blockNum = blockNum;

            // simply test remaining of line, There should be nothing left: - (AG)
            string testString;
            getline(game, testString);

            if (testString.length())
            {
              /**
               * we expect nothing after each block line
               * if so, exit parse, do not start game
               *  - (AG)
               */
              doneParsing = true;
            } // end if - (AG)
          }
          else
          {
            doneParsing = true; // wrong index size, exit parse, do not start game - (AG)
          }                     // end if - (AG)
        }                       // end while - (AG)
      }                         // end if - (AG)
    }                           // end if - (AG)
  }                             // end if - (AG)
  game.close();

  /**
   * if game is playing, parse was successful
   *  - (AG)
   */
  if (_gameStatus == 2)
  {
    /**
     * not necessary to delete file.
     * might be a nice feature to restart failed game in some way.
     *  - (AG)
     */
    remove(_fileName);
    return true;
  } // end if - (AG)
  return false;
} // end startSavedGame() - (AG)

void mineSweeper::saveGame()
{
  // if the game status indicates a completed game, save the game state to a file - (AG)
  if (_gameStatus == 2)
  {
    ofstream game;
    game.open(_fileName);

    {
      // write game size
      game << to_string(getSize_x()) << " " << to_string(getSize_y()) << '\n';

      // write each block into the file - (AG)
      int blockIndex;
      block defaultBlock; // << if a block has default states, then don't bother writing to a file - (AG)

      // run through each block, comparing to default vals, and saving if necessary: - (AG)
      for (blockIndex = 0; blockIndex < _gameBoard.size(); blockIndex++)
      {
        if (_gameBoard[blockIndex].isCovered != defaultBlock.isCovered ||
            _gameBoard[blockIndex].isFlagged != defaultBlock.isFlagged ||
            _gameBoard[blockIndex].isBomb != defaultBlock.isBomb ||
            _gameBoard[blockIndex].blockNum != defaultBlock.blockNum)
        {
          game << to_string(blockIndex) << " ";
          game << to_string(_gameBoard[blockIndex].isCovered) << " ";
          game << to_string(_gameBoard[blockIndex].isFlagged) << " ";
          game << to_string(_gameBoard[blockIndex].isBomb) << " ";
          game << to_string(_gameBoard[blockIndex].blockNum) << '\n';
        } // end if - (AG)
      }   // end for - (AG)

      game << "-1" << '\n'; // end of game board, mark with sentinel  value - (AG)
    }                       // end guard - (AG)

    game.close();
  }
  else
  {
    // remove file if no game to save. - (AG)
    remove(_fileName);
  } // end if

  // automatically set game to idle: - (AG)
  _gameStatus = -1;
} // end saveGame() - (AG)

bool mineSweeper::checkSavedGame()
{
  bool retVal = false;
  ifstream game(_fileName);

  // Check if the file is open and can be read - (AG)
  if (game.is_open())
  {
    int size_x, size_y;
    game >> size_x;
    game >> size_y;

    // Check if the file has not reached the end, indicating the presence of game board size information - (AG)
    if (!game.eof())
    {
      retVal = true;
    } // end if - (AG)
  }   // end if - (AG)

  return retVal;
} // end checkSavedGame() - (AG)

void mineSweeper::move(uint8_t coord_x, uint8_t coord_y, bool action)
{
  _move(_getIndex(coord_x, coord_y), action);
  _updateGameStatus();
} // end move() - (AG)

void mineSweeper::move(int index, bool action)
{
  _move(index, action);
  _updateGameStatus();
} // end move - (AG)

mineSweeper::block mineSweeper::getBlock(uint8_t coord_x, uint8_t coord_y)
{
  // Find the index of the block corresponding to the given coordinates - (AG)
  int blockIndex = _getIndex(coord_x, coord_y);

  block retVal; // return block value. has default values normally - (AG)
  if (blockIndex != -1 && _gameStatus != -1)
  {
    retVal = _gameBoard[blockIndex];
  } // end if - (AG)

  return retVal;
} // end getBlock() - (AG)

uint8_t mineSweeper::getSize_x()
{
  return _boardSize_x;
} // end getSize_x - (AG)

uint8_t mineSweeper::getSize_y()
{
  return _boardSize_y;
} // end getSize_y - (AG)

int8_t mineSweeper::getStatus()
{
  return _gameStatus;
} // end getStatus - (AG)

void mineSweeper::_placeBombs()
{
  unsigned int indexCount;
  /**
   * set seed using time
   * we dont want the same bomb placement each time the game is launched
   *  - (AG)
   */
  srand(time(0));

  // run through each block and determine if a bomb needs to be placed: - (AG)
  for (indexCount = 0; indexCount < _gameBoard.size(); indexCount++)
  {
    // a low '_bombSpread' num results in a greater chance of a bomb being placed at a given block: - (AG)
    if (!(rand() % _bombSpread))
    {
      _gameBoard[indexCount].isBomb = true; // << set bomb - (AG)
    }                                       // end if - (AG)
  }                                         // end for - (AG)
} // end _placeBombs() - (AG)

void mineSweeper::_placeNumbers()
{
  unsigned int blockIndex;

  for (blockIndex = 0; blockIndex < _gameBoard.size(); blockIndex++)
  {
    if (_gameBoard[blockIndex].isBomb) // Check if the block is a bomb - (AG)
    {
      // if block is a bomb, increment all valid adjacent blocks - (AG)
      /**
       * blockCode indicates with adjacent block is targeted
       * starting with top block and going around clockwise. eg.
       * blockcode = 0
       *  O X O
       *  O 0 O
       *  O O O
       * blockcode = 1
       *  O O X
       *  O 0 O
       *  O O O
       * ...thru 7
       *  - (AG)
       */
      uint8_t adjacentBlockCode;

      // iterate through each possible adjacent block code - (AG)
      for (adjacentBlockCode = 0; adjacentBlockCode < 8; adjacentBlockCode++)
      {
        int adjacentIndex = _getAdjacent(blockIndex, adjacentBlockCode); // get index of adjacent block using blockCode - (AG)

        if (adjacentIndex > -1) // valid adjacent block - (AG)
        {
          _gameBoard[adjacentIndex].blockNum++; // << inc hint number - (AG)
        }                                       // end if - (AG)
      }                                         // end for - (AG)
    }                                           // end if - (AG)
  }                                             // end for - (AG)
} // end _placeNumbers() - (AG)

void mineSweeper::_updateGameStatus()
{
  bool allBlocksUncovered = true; // flag to track if all blocks are uncovered - (AG)
  bool allBombsFlagged = true;    // flag to track if all bombs are flagged - (AG)
  bool bombIsHit = false;         // flag to track if a bomb is hit - (AG)

  unsigned int index;

  // iterate through each block in the game board - (AG)
  for (index = 0; index < _gameBoard.size(); index++)
  {
    // check if a bomb block is uncovered, indicating a hit - (AG)
    if (_gameBoard[index].isBomb && !_gameBoard[index].isCovered)
    {
      bombIsHit = true;
    } // end if - (AG)
    // check if a non-bomb block is covered, indicating not all blocks are uncovered - (AG)
    if (!_gameBoard[index].isBomb && _gameBoard[index].isCovered)
    {
      allBlocksUncovered = false;
    } // end if - (AG)
    // check if the flags of bomb and non-bomb blocks are inconsistent, indicating not all bombs are flagged - (AG)
    if ((_gameBoard[index].isBomb && !_gameBoard[index].isFlagged) ||
        (!_gameBoard[index].isBomb && _gameBoard[index].isFlagged && _gameBoard[index].isCovered))
    {
      allBombsFlagged = false;
    } // end if - (AG)
  }   // end for - (AG)

  // update the game status accordingly - (AG)
  if (bombIsHit)
  {
    // game won - (AG)
    _gameStatus = 0;
  }
  else if (allBombsFlagged || allBlocksUncovered)
  {
    // game won - (AG)
    _gameStatus = 1;
  } // end if - (AG)
} // end _updateGameStatus - (AG)

void mineSweeper::_move(int index, bool action)
{
  // validate input, do nothing otherwise - (AG)
  if (index >= 0 && index < _gameBoard.size())
  {
    if (action) // flag command - (AG)
    {
      _gameBoard[index].isFlagged = !_gameBoard[index].isFlagged;
    }
    else // dig command - (AG)
    {
      _gameBoard[index].isCovered = false; // uncover origin block by default - (AG)

      // do adjacent block uncover only if origin block is not a bomb or hint block - (AG)
      if (!_gameBoard[index].isBomb && _gameBoard[index].blockNum == 0)
      {
        // run through each of the 8 adjacent block and uncover if possible - (AG)
        int8_t blockCode;
        for (blockCode = 0; blockCode < 8; blockCode++)
        {
          int adjacentIndex = _getAdjacent(index, blockCode);

          if (adjacentIndex != -1 && _gameBoard[adjacentIndex].isCovered) // validate adjacent block index - (AG)
          {
            this->move(adjacentIndex, false); // call recursively on new block - (AG)
          }                                   // end if - (AG)
        }                                     // end for - (AG)
      }                                       // end if - (AG)
    }                                         // end if - (AG)
  }                                           // end if - (AG)
} // end _move() - (AG)

int mineSweeper::_getAdjacent(unsigned int index, uint8_t blockCode)
{
  // signed int because we might need to go into negatives - (AG)
  // convert index into x and y coordinates: - (AG)
  int16_t coord_y = index / _boardSize_x;
  int16_t coord_x = index % _boardSize_x;
  int returnIndex = 0;

  // make sure initial index is valid: - (AG)
  if (index >= _gameBoard.size())
  {
    returnIndex = -1;
  }
  else
  {
    // use x and y to find adjacent based on blockcode: - (AG)
    switch (blockCode)
    {
    case 0:
      coord_y--;
      // O X O <<
      // O 0 O
      // O O O - (AG)
      break;
    case 1:
      coord_y--;
      coord_x++;
      // O O X <<
      // O 0 O
      // O O O - (AG)
      break;
    case 2:
      coord_x++;
      // O O O
      // O 0 X <<
      // O O O - (AG)
      break;
    case 3:
      coord_y++;
      coord_x++;
      // O O O
      // O 0 O
      // O O X << - (AG)
      break;
    case 4:
      coord_y++;
      // O O O
      // O 0 O
      // O X O << - (AG)
      break;
    case 5:
      coord_y++;
      coord_x--;
      // O O O
      // O 0 O
      // X O O << - (AG)
      break;
    case 6:
      coord_x--;
      // O O O
      // X 0 O <<
      // O O O - (AG)
      break;
    case 7:
      coord_y--;
      coord_x--;
      // X O O <<
      // O 0 O
      // O O O - (AG)
      break;
    default:
      returnIndex = -1; // invalid blockcode - (AG)
      break;
    } // end switch - (AG)
  }   // end if - (AG)

  // determine return index value... - (AG)
  if (returnIndex != -1)
  {
    if (coord_x < 0 || coord_y < 0 || coord_x >= _boardSize_x || coord_y >= _boardSize_y)
    {
      // out of bounds! - (AG)
      returnIndex = -1;
    }
    else
    {
      // valid index: - (AG)
      returnIndex = _getIndex(coord_x, coord_y);
    } // end if - (AG)
  }   // end if - (AG)
  return returnIndex;
} // end _getAdjacent() - (AG)

int mineSweeper::_getIndex(uint8_t coord_x, uint8_t coord_y)
{
  int returnIndex;
  if (coord_x < 0 || coord_y < 0 || coord_x >= _boardSize_x || coord_y >= _boardSize_y)
  {
    // out of bounds! invalid - (AG)
    returnIndex = -1;
  }
  else
  {
    // valid, convert to index: - (AG)
    returnIndex = ((int)coord_y * (int)_boardSize_x) + (int)coord_x;
  } // end if - (AG)

  return returnIndex;
} // end _getIndex - (AG)

mineSweeper::~mineSweeper()
{
  _gameBoard.clear();
} // end ~mineSweeper - (AG)