/******************************************************************************\
|File:  /src/headers/_nconfile.h											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef NCONFILE_H
#define NCONFILE_H

#include <iostream>
#include <string>
#include "mutex.h"
#include "_fileioexception.h"

/***************************************************************\
|Class[ NConFile ]:												|
|	The primary use of this class is to open, read from, write	|
|	to, and close files open by the programmer.					|
\***************************************************************/
class NConFile:public Mutex{
	private:
		String _fileLocation;
		long _size;
		FILE* _handle;

	public:
		enum Mode{READ_ONLY,READ_WRITE};
		
	private: 
		Mode _mode;

	public:
		/***************************************************************\
		|Method[ NConFile ]:											|
		|	Constructor;  Opens file at the location pointed to by the	|
		|	file parameter in the specified mode.						|
		|Throws[ FileIOException ]:										|
		|	Exception thrown if:										|
		|		File pointed to by 'file' does not exist and			|
		|		createIfNotExists is false								|
		|		An attempt to open a file in READ_ONLY mode is made on	|
		|		a file that does not exist.								|
		|																|
		|Param[ file ]:													|
		|	Location of the file to be opened.							|
		|Param[ mode ]:													|
		|	Mode to open the file in.  Default mode is read only.		|
		|Param[ createIfNotExists ]:									|
		|	Only valid if mode is READ_WRITE;  Allows the creation of	|
		|	The file if it doesn't already exist.  Default is false.	|
		\***************************************************************/
		NConFile(String file,Mode mode=READ_ONLY,bool createIfNotExists=false);

		/***************************************************************\
		|Method[ ~NConFile ]:											|
		|	Closes the file.											|
		\***************************************************************/
		~NConFile();

		/***************************************************************\
		|Method[ changeFileMode ]:										|
		|	Reopens the file in the mode specified.						|
		|																|
		|Param[ mode ]:													|
		|	Mode to change the file to.									|
		\***************************************************************/
		void changeFileMode(Mode mode);
		
		/***************************************************************\
		|Method[ append ]:												|
		|	Adds the string of characters specified by stringToAppend	|
		|	to the end of the file.										|
		|Throws[ FileIOException ]										|
		|																|
		|Param[ stringToAppend ]:										|
		|	Character or byte string to append onto the file.  Does		|
		|	not need to end with a NULL byte.							|
		|Param[ numchars ]:												|
		|	Number of characters to append from stringToAppend onto		|
		|   the end of the file.										|
		\***************************************************************/
		void append(String stringToAppend,unsigned numchars);

		/***************************************************************\
		|Method[ getString ]:											|
		|	Gets a string of specified length from the file starting at	|
		|	startPosition.												|
		|Return:														|
		|	Returns String of specified length from the file.			|
		|																|
		|Param[ startPosition]:											|
		|	Number of bytes into the file to start reading.  Must be	|
		|	less than the number of bytes in the file.					|
		|Param[ numChars ]:												|
		|	Number of bytes to read from the file.						|
		\***************************************************************/
		String getString(long startPosition,unsigned numChars);

		/***************************************************************\
		|Method[ getLine ]:         									|
		|   Gets a line of text from the opened file.  This file is     |
		|   intended to be used just after the file is opened.          |
		|Return:                                                        |
		|   Returns a copy of the text that was pulled from the file.   |
        |                                                               |
        |Param[ delimeter ]:                                            |
        |   Stops reading in data once the specified delimeter is       |
        |   reached.                                                    |
        |Param[ numChars ]                                              |
        |   Will cause getLine to stop reading in characters once       |
        |   numChars have been read in.                                 |
		\***************************************************************/
		String getLine(char delimeter='\n',unsigned numChars=0);

		/***************************************************************\
		|Method[ clear ]:												|
		|	Deletes everything in the file.								|
		\***************************************************************/
		void clear();

		/***************************************************************\
		|Method[ getFileLocation ]:										|
		|	Standard getter method.										|
		|Return:														|
		|	returns private variable fileLocation.						|
		\***************************************************************/
		String getFileLocation() const;

		/***************************************************************\
		|Method[ getSize ]:												|
		|	Standard getter method.										|
		|Return:														|
		|	returns private variable _size.								|
		\***************************************************************/
		unsigned getSize() const;

		friend class FileManager;
};

#endif
