/******************************************************************************\
|File:  /src/headers/_filemanager.h         								   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <string>
#include "_nconfile.h"
#include "exception.h"

/***************************************************************\
|Class[ FileManager ]:											|
|	A simple class built make make file handle management just a|
|	bit easier.													|
\***************************************************************/
class FileManager{
	private:
		List<NConFile*> _handles;

	public:
		/***************************************************************\
		|Method[ ~FileManager ]:										|
		|	Iterates through all the files in the handles variable and	|
		|	closes and deletes each of them.							|
		\***************************************************************/
		~FileManager();

		/***************************************************************\
		|Method[ fetchFileHandle ]:										|
		|   Opens and maintains a handle to the file at fileLocation.	|
		|Return:														|
		|	Returns the a handle to one of the NConFile objects in the	|
		|	'_handles' list.  If the file cannot be opened, NULL will be|
		|   returned.                                                   |
		|                                                               |
		|Param[ fileLocaiton ]:                                         |
		|   Location on the system of the file to be opened.            |
		|Param[ mode ]:                                                 |
		|   Mode with in which the file will be opened.                 |
		|Param[ createIfNotExists ]:                                    |
		|   Defines if the file should be created if it doesn't exist.  |
		\***************************************************************/
		NConFile* fetchFileHandle(String fileLocation,NConFile::Mode mode=NConFile::READ_ONLY,bool createIfNotExists=false);
};

extern FileManager FileMan;

#endif
