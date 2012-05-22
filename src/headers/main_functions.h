/******************************************************************************\
|File:  /src/headers/main_functions.h										   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef MAINF_H
#define MAINF_H

#include <iostream>
#include <dirent.h>
#include "string.h"
#include "list.h"

/***************************************************************\
|Function[ RunConsole ]:										|
|	Thread Runner;  The main thread in NCon, which, after the   |
|   system has been loaded, should be running at all times.     |
\***************************************************************/
void* RunConsole(void* args);

/***************************************************************\
|Function[ parseCommand ]:                                      |
|   This function is only intended to be used with RunConsole.  |
|   Takes a user input command (statement), and seperates it    |
|   into it's individual components.                            |
|                                                               |
|Param[ statement ]:                                            |
|   Full user input.                                            |
|Param[ command ]:                                              |
|   Command part of user input.                                 |
|Param[ arguments ]:                                            |
|   List of each individual argument to be passed to command.   |
\***************************************************************/
void parseCommand(String statement,String& command,List<String>& arguments);

/***************************************************************\
|Function[ isPrintableChar ]:									|
|   A simple check to test whether or not the input character is|
|   able to be printed on teh screen.                           |
|Return:                                                        |
|   True if the character can be printed, or false, otherwise.  |
\***************************************************************/
bool isPrintableChar(const char& ch);

/***************************************************************\
|Function[ getChar ]:                                           |
|   Gets a character from the io buffer without waiting for the |
|   user to press enter.                                        |
|Return:                                                        |
|   Returns a copy of the character pressed by the user.        |
\***************************************************************/
char getChar();

/***************************************************************\
|Function[ getString ]:   										|
|	Gets a string from user input, and stores it in the buffer  |
|   provided.                                                   |
|Return:                                                        |
|   Returns the string input by the user.                       |
|                                                               |
|Param[ maxCharsToGet ]:                                        |
|   This is the max number of characters to return.  A value of |
|   zero will cause it to loop until 'delim' is reached.        |
|Param[ delim ]:                                                |
|   Stops user input if the delimiting character in 'delim' is  |
|   reached.                                                    |
|Param[ echo ]:                                                 |
|   If echo is true, any characters typed by teh user will be   |
|   printed to teh console.  Otherwise, they will be consumed   |
|   and not printed.                                            |
|Param[ base ]:                                                 |
|   Base to begin string with.                                  |
|Param[ console_command ]:                                      |
|   If this is NULL, all console commands will be ignored.      |
|   Otherwise, this should be a pointer to a string which will  |
|   store any console commands entered by the user.             |
\***************************************************************/
String getString(const unsigned maxCharsToGet=0,const char delim='\n',const bool echo=true,const String base="",String* console_command=NULL);

/***************************************************************\
|Function[ displayHelp ]:										|
|	Displays help information for this program.                 |
\***************************************************************/
void displayHelp();

/***************************************************************\
|Function[ displayWelcomeInfo ]:								|
|	Displays welcome information upon successful user login.    |
\***************************************************************/
void displayWelcomeInfo();

#endif