/******************************************************************************\
|File:  /src/bins/md/main.cpp												   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <iostream>
#include <sys/stat.h>
#include <string.h>
#include "../NCon.h"
#include "../../headers/system.h"

int main(int argc,char** argv){
	LoadNcon(argc,argv);
	
	bool invalidCharFound=false;

	if(argc<3){
		printf("The \"md\" command requires at least one argument.  The directory.  For usage, see \"md --help\".");
		return 1;
	}

	for(unsigned index=0;index<strlen(argv[argc-1]);index++){
		//check for invalid characters
		switch(argv[argc-1][index]){
			case '-':
			//case '/':
			case '\\':  //I know there's more than that.  Just putting the ones I can think of off the top of my head.
				invalidCharFound=true;
				printf("Your directory contained an illegal character.  Offending character:  %c\n",argv[argc-1][index]);
				break;
		}
	}
	if(!invalidCharFound){
		if(!System::makeDirectory(argv[argc-1])){
			printf("Directory could not be created.\n");
		}  //TODO:  perform error checking on this.
	}

	return 0;
}