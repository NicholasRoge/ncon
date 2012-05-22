/******************************************************************************\
|File:  /src/NCon/main.cpp													   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <iostream>
#include <string>
#include <cstdio>
#include "../headers/system.h"
#include "../headers/main_functions.h"
#include "../headers/user.h"
#include "../headers/thread.h"

using namespace std;

int main(int argc,char** argv){
	try{
		//Parse any arguments that may have been passed.
		for(int index=1;index<argc;index++){
			if(strcmp(argv[index],"--help")==0){
				displayHelp();
	
				return 0;
			}else if(strcmp(argv[index],"--install")==0){
				switch(System::install()){
					case 0:
						printf("NCon Subsystem was installed successfully!\n");
						return 0;
					case 1:
						printf("NCon Subsystem could not be installed because it appears to already be installed\non this system.\n");
						return -1;
					case 2:
						printf("\
	NCon Subsystem could not be installed because the directory structure could not\n\
	be created.  Please check that the current user has the correct permissions to\n\
	write to this directory.\n");
						return -1;
					default:
						printf("NCon Subsystem could not be installed because of an unknown reason.\n");
						return -1;
				}
			}
		}
	
		if(!System::is_installed()){
			printf("You must install this program first.  To do so, just type \"ncon --install\".");
			return -1;
		}
		
		System::LoadSystem();
	}catch(Exception e){
		printf("%s\n",(char*)e.getMessage());
	}
	
	Thread* mainThread=ThreadMan.getNewThread("Main Thread",RunConsole,NULL,true);
	mainThread->start();

	mainThread->waitForThread();

	System::UnloadSystem();

}

/*MASTER TODO SHEET:
 *
 *Add a variable to the Thread class to mark if it's critical or not.  If it is critical, the process must be allowed to complete before the program terminates.
 */
