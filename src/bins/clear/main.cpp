/******************************************************************************\
|File:  /src/bins/clear/main.cpp											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <iostream>
#include <string.h>
#include <stdio.h>
#include "../NCon.h"

int main(int argc,char** argv){
	LoadNcon(argc,argv);
    
	if(argc==3){
		if(strcmp(argv[2],"--help")==0||strcmp(argv[2],"--sys-help")==0){
			printf("\
Usage:  clear\n\
Description:\n\
    Clears the consoles screen.\n");

			return 0;  //This says the system help has been consumed.
		}
	}

	execl("/usr/bin/clear","/usr/bin/clear",NULL,NULL);
	
	return 0;
}