#ifndef cinParse_h
#define cinParse_h

#include <iostream>
#include <string>
#include <stdint.h> // << for 'uint8_t' - (AG)
using namespace std;

class cinParse
{
public:
  // structs to help organize data: - (AG)
  struct size_struct
  {
    int coord_x = -1;
    int coord_y = -1;
  };
  struct move_struct
  {
    int coord_x = -1;
    int coord_y = -1;

    bool commandAction = false; // false = dig, true = flag - (AG)
  };

  /**
   * input:
   *    - input is read from standard input (cin) using getline
   *    - the input is assumed to be a string representing commands and parameters
   *
   * process:
   *    - this is a blocking function, it will wait until any character is entered via cin
   *    - inputs are converted to lowercase
   *    - function determines if command is one of two types:
   *      - character command
   *      - move command
   *      otherwise, invalid input is flagged
   *    - depending on the input, the appropriate flags are set within the class,
   *      getter methods can be used to read flags.
   *      all flags will retain previous states until 'parse()' is called again
   *
   * output:
   *   - the method updates class member variables (_charCommand, _isCharCommand, _startSize, _moveCommand, _isMoveCommand)
   *   - these variables store the parsed commands and parameters for further processing.
   *   - no explicit output is provided by this method.
   *
   * notes:
   *   - this class simply parses input, it will not validate data or print error messages
   *  - (AG)
   */
  void parse();

  /**
   *  input: none
   *  proccess: none
   *  output: (boolean) is character command flag from previous parse
   *  - (AG)
   */
  bool isCharCommand();
  /**
   *  input: none
   *  proccess: none
   *  output: (boolean) is move command flag from previous parse
   *  - (AG)
   */
  bool isMoveCommand();

  /*
    input: none
    proccess: check if isCharCommand is true, return error otherwise
    output: (int) char command flag from previous parse
           -1 = error / no char command
            0 = quit command
            1 = start game command
            2 = start game command with size
            3 = continue game command
            4 = help command
             - (AG)
  */
  int getCharCommand();
  /*
    input: none
    proccess: check if getCharCommand equals 'start game command with size', return default values otherwise
    output: (size_struct) structure with size in x and y coordinates
     - (AG)
  */
  size_struct getStartSize();

  /*
    input: none
    proccess: check if isMoveCommand is set, return default values otherwise
    output: (move_struct) structure with size in x and y coordinates and action command
     - (AG)
  */
  move_struct getMoveCommand();

private:
  // structure to hold start size and move command values: - (AG)
  size_struct _startSize;
  move_struct _moveCommand;

  // flags to be set during parsing: - (AG)
  bool _isCharCommand = false;
  bool _isMoveCommand = false;

  /*
    _charCommand:
     -1 = error / no char command
      0 = quit command
      1 = start game command
      2 = start game command with size
      3 = continue game command
      4 = help command
       - (AG)
  */
  int _charCommand = -1;
};  // end class cinParse

#endif  // end #ifndef cinParse_h - (AG)