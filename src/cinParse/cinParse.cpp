#include "cinParse.h"
using namespace std;

void cinParse::parse()
{
  // clear all flags: - (AG)
  _isCharCommand = false; // indicates whether the command is a single character command - (AG)
  _isMoveCommand = false; // indicates whether the command is a move command - (AG)

  string input;
  getline(cin, input); // retrieve using standard intput - (AG)

  if (input.length()) // make sure input not empty - (AG)
  {
    // set all chars to lowercase: - (AG)
    int charCount;
    for (charCount = 0; charCount < input.length(); charCount++)
    {
      input[charCount] = tolower(input[charCount]);
    } // end for - (AG)

    // if input is a single char, check which: - (AG)
    if (input.length() == 1)
    {
      switch (input[0])
      {
      case 'x':
        // quit command - (AG)
        _charCommand = 0;
        _isCharCommand = true;
        break;
      case 's':
        // start command - (AG)
        _charCommand = 1;
        _isCharCommand = true;
        break;
      case 'c':
        // continue command - (AG)
        _charCommand = 3;
        _isCharCommand = true;
        break;
      case '?':
        // help command - (AG)
        _charCommand = 4;
        _isCharCommand = true;
        break;
      } // end switch - (AG)
    }
    else
    {
      ////// parse start size ////// - (AG)
      {
        string start_string = input;
        if (start_string[0] == 's' && start_string[1] == ' ')
        {
          start_string = start_string.substr(2); // Remove "s " from the beginning. - (AG)

          if (isdigit(start_string[0]))
          {
            // use try catch for stoi: - (AG)
            try
            {
              // set x size - (AG)
              _startSize.coord_x = stoi(start_string);

              // Remove digits from the string until a non-digit character is found - (AG)
              start_string = start_string.substr(1);
              while (start_string.length() && isdigit(start_string[0]))
              {
                start_string = start_string.substr(1); // remove one character - (AG)
              }                                        // end while - (AG)

              // find and remove space separating the start coordinates: - (AG)
              if (start_string.length() && start_string[0] == ' ')
              {
                start_string = start_string.substr(1); // remove space - (AG)

                if (isdigit(start_string[0]))
                {
                  // use try catch for stoi: - (AG)
                  try
                  {
                    // set y size - (AG)
                    _startSize.coord_y = stoi(start_string);

                    // Remove digits from the string until a non-digit character is found - (AG)
                    start_string = start_string.substr(1);
                    while (start_string.length() && isdigit(start_string[0]))
                    {
                      start_string = start_string.substr(1); // remove one character - (AG)
                    }                                        // end while - (AG)

                    // we dont expect any more characters, validate: - (AG)
                    if (!start_string.length())
                    {
                      // valid start size command found... - (AG)
                      _charCommand = 2;
                      _isCharCommand = true;
                    } // end if - (AG)
                  }
                  catch (exception &err)
                  {
                    // failed to convert to int, simply ignore - (AG)
                  } // end try-catch - (AG)
                }   // end if - (AG)
              }     // end if - (AG)
            }
            catch (exception &err)
            {
              // failed to convert to int, simply ignore - (AG)
            } // end try-catch - (AG)
          }   // end if - (AG)
        }     // end if - (AG)
      }       // end guard - (AG)

      ////// parse move command ////// - (AG)
      {
        /*
          alphabet and number coordinate can be in either order, - (AG)
          check both. - (AG)
          x coord = alphabetical - (AG)
          y coord = numerical - (AG)
        */
        string move_string = input;
        if (move_string[0] >= 'a' && move_string[0] <= 'z') // alphabetical first coordinate - (AG)
        {
          // set x coord - (AG)
          _moveCommand.coord_x = move_string[0] - 'a';
          move_string = move_string.substr(1); // remove alphanumerical char - (AG)

          if (isdigit(move_string[0])) // validate next char is digit - (AG)
          {
            try
            {
              // set y coord - (AG)
              _moveCommand.coord_y = stoi(move_string);

              // Remove digits from the string until a non-digit character is found - (AG)
              move_string = move_string.substr(1);
              while (move_string.length() && isdigit(move_string[0]))
              {
                move_string = move_string.substr(1); // remove one character - (AG)
              }                                      // end while - (AG)

              /*
                two coordinates have been found, - (AG)
                one of two options left now. - (AG)
                an action command(d = dig, f = flag) - (AG)
                or a default action command(dig) - (AG)
              */
              if (move_string.length() == 1)
              {
                // check whihc action command selected: - (AG)
                switch (move_string[0])
                {
                case 'd':
                  _moveCommand.commandAction = false;
                  _isMoveCommand = true; // set flag - (AG)
                  break;
                case 'f':
                  _moveCommand.commandAction = true;
                  _isMoveCommand = true; // set flag - (AG)
                  break;
                } // end switch - (AG)
              }
              else if (!move_string.length())
              {
                // set default action command - (AG)
                _moveCommand.commandAction = false;
                _isMoveCommand = true; // set flag - (AG)
              }                        // end if - (AG)
            }
            catch (exception &err)
            {
              // failed to convert to int, simply ignore - (AG)
            } // end try-catch - (AG)
          }   // end if - (AG)
        }
        else // numerical first coordinate - (AG)
        {
          if (isdigit(move_string[0]))
          {
            try
            {
              // set y coord - (AG)
              _moveCommand.coord_y = stoi(move_string);

              // Remove digits from the string until a non-digit character is found - (AG)
              move_string = move_string.substr(1);
              while (move_string.length() && isdigit(move_string[0]))
              {
                move_string = move_string.substr(1); // remove one character - (AG)
              }                                      // end while - (AG)

              if (move_string[0] >= 'a' && move_string[0] <= 'z')
              {
                // set x coord - (AG)
                _moveCommand.coord_x = move_string[0] - 'a';
                move_string = move_string.substr(1); // remove alphanumerical char - (AG)

                /*
                  two coordinates have been found,
                  one of two options left now.
                  an action command(d = dig, f = flag)
                  or a default action command(dig)
                   - (AG)
                */
                if (move_string.length() == 1)
                {
                  // check whihc action command selected: - (AG)
                  switch (move_string[0])
                  {
                  case 'd':
                    _moveCommand.commandAction = false;
                    _isMoveCommand = true; // set flag - (AG)
                    break;
                  case 'f':
                    _moveCommand.commandAction = true;
                    _isMoveCommand = true; // set flag - (AG)
                    break;
                  } // end switch - (AG)
                }
                else if (!move_string.length())
                {
                  // set default action command - (AG)
                  _moveCommand.commandAction = false;
                  _isMoveCommand = true; // set flag - (AG)
                }                        // end if - (AG)
              }                          // end if - (AG)
            }
            catch (exception &err)
            {
              // failed to convert to int, simply ignore - (AG)
            } // end try-catch - (AG)
          }   // end if - (AG)
        }     // end if - (AG)
      }       // end guard - (AG)
    }         // end if - (AG)
  }           // end if - (AG)
} // end parse() - (AG)

bool cinParse::isCharCommand()
{
  return _isCharCommand;
} // end isCharCommand - (AG)
bool cinParse::isMoveCommand()
{
  return _isMoveCommand;
} // end isMoveCommand - (AG)
int cinParse::getCharCommand()
{
  // check if there is a valid char command: - (AG)
  if (_isCharCommand)
  {
    return _charCommand;
  }          // end if - (AG)
  return -1; // return default error - (AG)
} // end getCharCommand - (AG)
cinParse::size_struct cinParse::getStartSize()
{
  // check if there is a valid char command: - (AG)
  if (getCharCommand() == 2)
  {
    return _startSize;
  } // end if - (AG)

  // return default values: - (AG)
  size_struct temp = {-1, -1};
  return temp;
} // end getStartSize - (AG)

cinParse::move_struct cinParse::getMoveCommand()
{
  // check if there is a valid move command: - (AG)
  if (_isMoveCommand)
  {
    return _moveCommand;
  } // end if - (AG)

  // return default values: - (AG)
  move_struct temp = {-1, -1, false};
  return temp;
} // end getMoveCommand - (AG)