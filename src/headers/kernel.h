/******************************************************************************\
|File:  /src/headers/kernel.h   											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef KERNEL_H
#define KERNEL_H

#include <iostream>
#include <string>
#include <cstdio>
#include "exception.h"
#include "list.h"
#include "string.h"

const unsigned MAX_ENV_VARS=1000;
const unsigned MAX_ENV_KEY_LENGTH=101;
const unsigned MAX_ENV_VALUE_LENGTH=901;

const unsigned MAX_HISTORY_SIZE=100;  //This will eventually be in a config file.

class User;

typedef unsigned char byte;

enum{CURRENT_USER_KEY,ENVIRONMENT_KEYS_KEY,ENVIRONMENT_VALUES_KEY,CURRENT_SESSION_KEY};
extern User* CurrentUser;

extern char* EnvironmentKeys;
extern char* EnvironmentValues;

/***************************************************************\
|Function[ FileExists ]:										|
|	Checks to see that the specified file exists;  Will also put|
|   a character indicating the type of file pathToFile is.      |
|Return:														|
|	Returns true, if the file exists, and false, if it doesn't.	|
|Throws[ Exception ]:											|
|	Throws generic exception if:                                |
|       Path to File is empty.                                  |
|																|
|Param[ pathToFile ]:											|
|	Path to the file to be checked.								|
|Param[ fileType ]:                                             |
|   Pointer to a char to put the file type in.                  |
\***************************************************************/
bool FileExists(const String pathToFile,char* fileType=NULL);

/***************************************************************\
|Function[ getAbsoluteLocation ]:								|
|	Takes a relative directory (containing paths such as '.' and|
|	'..', and turns them into either a path based relative to	|
|	either NCon, or OS that NCon is running on.					|
|Throws[ Exception ]:											|
|	Throws generic exception if:                                |
|       Relative path is empty.                                 |
|       NCon's system has been loaded, but the SHELL environment|
|         variable wasn't found.                                |
|																|
|Param[ relativeLocation ]:										|
|	A path which needs to be resolved.							|
|Param[ relativeToRealOS ]:										|
|	If this parameter is true, then the resolved path will be	|
|	relative to the OS Ncon is running on.  Otherwise, the path	|
|	will be relative to NCon.									|
\***************************************************************/
String getAbsoluteLocation(const String relativePath,bool relativeToRealOS=true);

/***************************************************************\
|Function[ setEnv ]:											|
|	Sets an evironment variable to the value specified.			|
|Throws[ Exception ]:											|
|	Throws generic exception if:                                |
|       The environment Variable hasn't been loaded yet.        |
|       Key length is greater than MAX_ENV_KEY_LENGTH.          |
|       Value length is greater than MAX_ENV_VALUE_LENGTH.      |
|       There's are already MAX_ENV_VARS in memory.             |
|																|
|Param[ key ]:													|
|	Specifies which environment variable you wish to get.		|
|Param[ value ]:												|
|	Value that the evironment variable should be set to.		|
\***************************************************************/
void setEnv(const String key,const String value);

/***************************************************************\
|Function[ getEnv ]:											|
|	Gets the value of an evironment variable specified by 'key'.|
|Return:                                                        |
|   Returns the value which 'key' holds.                        |
|Throws[ Exception ]:											|
|	Throws generic exception if:                                |
|       The environment Variable hasn't been loaded yet.        |
|       Key is empty.                                           |
|       Environment variable doesn't exist.                     |
|																|
|Param[ key ]:													|
|	Specifies which environment variable you wish to get.		|
\***************************************************************/
String getEnv(const String key);

/***************************************************************\
|Function[ removeEnv ]:											|
|	Removes the evironment variable specified by 'key'.         |
|Throws[ Exception ]:											|
|	Throws generic exception if:                                |
|       The environment variable hasn't been loaded yet.        |
|       Key is empty.                                           |
|       Environment variable doesn't exist.                     |
|																|
|Param[ key ]:													|
|	Specifies which environment variable you wish to remove.	|
\***************************************************************/
void removeEnv(const String key);

#endif
