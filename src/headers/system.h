/******************************************************************************\
|File:  /src/headers/system.h   											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <dirent.h>
#include <iostream>
#include <string.h>
#include "exception.h"
#include "list.h"

class NConFile;

/***************************************************************\
|Class[ System ]:       							        	|
|   Contains most of the commands to be used in teh normal      |
|   execution of NCon.                                          |
\***************************************************************/
class System{
	private:
		static unsigned _install_makeSystemDirectoryTree();

		static unsigned _install_addSystemFiles();

		static void* _ExecuteCommand(void* args);
		static void* _ListenForCtrlC(void* args);

		static void* _MapMemory(void* args);
		
		static bool _is_loaded;

	public:
		enum SysFiles{
			USER_DB,
			ENV
		};

		static NConFile* getSysFile(SysFiles file);

		static bool makeDirectory(const String directory,bool makeNecessaryParents=true,bool relativeToCurrentOS=true);

		static bool is_installed();

		static unsigned install();

		static unsigned LoadSystem();

		static unsigned UnloadSystem();

		static void login_user(const String username,const String password);

		static int ExecuteCommand(const String command,const List<String> arguments);
		
		static bool isLoaded();
};

class LoginException:public Exception{
	public:
		enum ErrorCode{
		};

		LoginException(String message,unsigned eCode);
};

class ExecutionException:public Exception{
	public:
		enum ErrorCode{
			COMMAND_NOT_FOUND,
			SESSION_ERROR
		};

		ExecutionException(ErrorCode code);
		ExecutionException(String message,unsigned eCode);
};

#endif
