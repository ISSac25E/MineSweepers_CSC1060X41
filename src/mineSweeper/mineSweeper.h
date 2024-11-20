#ifndef mineSweeper_h
#define mineSweeper_h

#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <ctime>    // << for random number seed generation - (AG)
#include <stdint.h> // << for 'uint8_t' - (AG)
using namespace std;

class mineSweeper
{
public:
  /**
   * struct for each individual block
   *   holds the state of each block
   *   contains default states of each block
   *   this struct is used by the game vector
   *  - (AG)
   */
  struct block
  {
    bool isCovered = true;
    bool isFlagged = false;

    bool isBomb = false;
    uint8_t blockNum = 0;
  };

  /**
   * input: size_x - the width of the game board
   *        size_y - the height of the game board
   * process: Initializes a new game by setting the size of the game board, clearing the existing game board,
   *          placing bombs randomly on the board, placing numbers indicating the number of adjacent bombs for each non-bomb block
   * output: None (void function)
   *  - (AG)
   */
  void startNewGame(uint8_t, uint8_t);
  /**
   * input: None
   * process: Opens a file to load a saved game,
   *          reads game board size, parses game board data from the file.
   *          updates the game state accordingly, start game
   * output: Returns true if the saved game was successfully loaded and parsed, otherwise returns false
   *  - (AG)
   */
  bool startSavedGame();

  /**
   * Saves the current game state to a file if the game status indicates a completed game.
   *
   * input: none
   * process: if the game is completed, opens the file and writes game board data to it, otherwise deletes the file.
   * output: none
   *  - (AG)
   */
  void saveGame();
  /**
   * input: none
   * process: checks if a saved game file exists with game board size information.
   *          opens the file, reads game board size, and returns true if size data exists.
   * output: returns true if a saved game file exists with game board size information, otherwise false.
   *  - (AG)
   */
  bool checkSavedGame();

  /**
   * overloaded!
   * input: coord_x, coord_y: the coordinates of the block to perform the move on
   *        action: a bool indicating whether to flag the block (true) or uncover it (false)
   * process: converts coordinates into index and runs parent _move function
   *          updates the game-play status
   * output: None (void function)
   *  - (AG)
   */
  void move(uint8_t, uint8_t, bool);
  /**
   * overloaded!
   * input: index: the index of the block to perform the move on
   *        action: a bool indicating whether to flag the block (true) or uncover it (false)
   * process: runs parent _move function
   *          updates the game-play status
   * output: None (void function)
   *  - (AG)
   */
  void move(int, bool);
  /**
   * retrieves the block struct at the specified coordinates on the game board.
   *
   * input: coord_x - the x-coordinate of the block
   *        coord_y - the y-coordinate of the block
   * process: finds the index of the block corresponding to the given coordinates.
   *          if the block index is valid and the game is not in an uninitialized state,
   *          retrieves the block data from the game board.
   * output: returns the block data as a struct at the specified coordinates,
   *          OR a default block if the coordinates are invalid or the game is uninitialized.
   *  - (AG)
   */
  block getBlock(uint8_t, uint8_t);

  /**
   * input: None
   * process: None
   * output: Returns the width of the game board.
   *  - (AG)
   */
  uint8_t getSize_x();
  /**
   * input: None
   * process: None
   * output: Returns the height of the game board.
   *  - (AG)
   */
  uint8_t getSize_y();

  /**
   * input: None
   * process: None
   * output: Returns the state of the game
   * -1 = idle, n/a
   * 0 = game lost
   * 1 = game won
   * 2 = game playing
   *  - (AG)
   */
  int8_t getStatus();

  // process: clear vector - (AG)
  ~mineSweeper();

private:
  // vector with all blocks: - (AG)
  vector<block> _gameBoard;

  const char *_fileName = "cache.dat"; // file where game data is stored - (AG)
  const uint8_t _bombSpread = 10;      // larger number means more spread, must be larger than 0! - (AG)
  uint8_t _boardSize_x;
  uint8_t _boardSize_y;

  int8_t _gameStatus = -1; // -1 na, 1 = game won, 0 = game lost, 2 = playing - (AG)

  /**
   * Input: None
   * Process: Iterates over the elements of _gameBoard vector, generating a random number for each element
   * and setting its isBomb flag to true if the random number modulo _bombSpread evaluates to true.
   * Output: None (void function)
   *  - (AG)
   */
  void _placeBombs();

  /**
   * Input: None
   * Process: Iterates over each block in the _gameBoard vector. For each block that contains a bomb,
   *          it determines the adjacent blocks and increments their blockNum value, indicating the number
   *          of adjacent bombs.
   * Output: None (void function)
   *  - (AG)
   */
  void _placeNumbers();
  /**
   * Input: None
   * Process: Checks the current state of the game board to determine the game status.
   *          Sets _gameStatus to 0 if a bomb is uncovered, to 1 if all bombs are correctly flagged
   *          or all non-bomb blocks are uncovered; otherwise, leaves _gameStatus unchanged.
   * Output: None (void function)
   *  - (AG)
   */
  void _updateGameStatus();
  /**
   * Input: index - the index of the block to perform the action on
   *        action - a boolean indicating whether to flag the block (true) or uncover it (false)
   * Process: Flags or uncovers the block specified by the index parameter.
   *          If the action is to uncover a non-bomb block with no adjacent bombs, recursively uncovers
   *          adjacent blocks until a block with adjacent bombs is encountered.
   * Output: None (void function)
   *  - (AG)
   */
  void _move(int, bool);

  /**
   * Input: index - the index of the block for which adjacent block index is to be obtained
   *        blockCode - a code (0-7) representing one of the eight adjacent blocks
   * Process: Determines the index of the adjacent block based on the index and blockCode parameters.
   * Output: Returns the index of the adjacent block if it exists and is within the game board boundaries;
   *         otherwise, returns -1 to indicate an invalid adjacent block.
   *  - (AG)
   */
  int _getAdjacent(unsigned int, uint8_t);
  /**
   * Input: coord_x - the x-coordinate of the block
   *        coord_y - the y-coordinate of the block
   * Process: Determines the index of the block in the one-dimensional game board vector based on its coordinates.
   * Output: Returns the index of the block if it exists within the game board boundaries;
   *         otherwise, returns -1 to indicate an invalid block index.
   *  - (AG)
   */
  int _getIndex(uint8_t, uint8_t);
};  // end class mineSweeper - (AG)

#endif // end '#ifndef mineSweeper_h' - (AG)