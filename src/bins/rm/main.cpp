/******************************************************************************\
|File:  /src/bins/rm/main.cpp												   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include "../NCon.h"
#include "../../headers/string.h"

int main(int argc,char** argv){
	LoadNcon(argc,argv);
	
	char file_type;
	
	if(argc>0&&(strlen(argv[argc-1])>0&&argv[argc-1][0]!='-')){
		String objectToDelete=getAbsoluteLocation(argv[argc-1]);

		if(FileExists(objectToDelete,&file_type)){			
			switch(file_type){
				case 'D':
					if(rmdir(objectToDelete)==-1){
						switch(errno){
							case EACCES:
								printf("The user which is executing NCon does not have the correct permissions on the host system to perform this action.\n");
								break;
							case EBUSY:
								printf("This directory is being used by another process and cannot be deleted.\n");
								break;
							case ELOOP:
								printf("The path provided contains too many symbolic links.\n");
								break;
							case ENAMETOOLONG:
								printf("The path provided was too long.\n");
								break;
							default:
								printf("An error was encountered, but rm doesn't know how to resolve it.  ERRNO:  %i",errno);
								break;
						}
					}
					break;
				case 'R'://TODO:  check for a -d flag later on.
					if(unlink(objectToDelete)==-1){
						switch(errno){
							case EACCES:
								printf("The user which is executing NCon does not have the correct permissions on the host system to perform this action.\n");
								break;
							case EBUSY:
								printf("This file is being used by another process and cannot be deleted.\n");
								break;
							case ELOOP:
								printf("The path provided contains too many symbolic links.\n");
								break;
							case ENAMETOOLONG:
								printf("The path provided was too long.\n");
								break;
							default:
								printf("An error was encountered, but rm doesn't know how to resolve it.  ERRNO:  %i",errno);
								break;
						}
					}
					break;
				default:
					break;
			}
						
			/*switch(errno){
				//TODO:  Make this error list
			}*/
		}else{
			printf("Directory or file \"%s\" does not exist.\n",argv[argc-1]);
		}
	}
	
	return 0;
}