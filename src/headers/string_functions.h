/******************************************************************************\
|File:  /src/headers/string_functions.h										   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef STRINGF_H
#define STRINGF_H

#include <iostream>
#include "string.h"
#include "list.h"

/***************************************************************\
|Function[ implode ]:   										|
|	Concatenates the input List, adding the char 'implodeOn'    |
|   between each element.                                       |
|Return:                                                        |
|   Returns the imploded string.                                |
|                                                               |
|Param[ explodedString ]:                                       |
|   A list of strings to be inploded.                           |
|Param[ implodeOn ]:                                            |
|   Character that should be put between each element of        |
|   explodedString upon implosion.                              |
\***************************************************************/
String implode(const List<String> explodedString,char implodeOn);

/***************************************************************\
|Function[ intToString ]:							        	|
|   Converts the input number to a string of text.              |
|Return:                                                        |
|   Returns the string version of numStr.                       |
\***************************************************************/
String intToString(unsigned numStr);

/***************************************************************\
|Function[ stringToInt ]:							        	|
|   Converts the input string to a number.                      |
|Return:                                                        |
|   Returns the number version of str.                          |
\***************************************************************/
unsigned stringToInt(String str);

#endif