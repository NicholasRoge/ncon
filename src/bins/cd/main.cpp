/******************************************************************************\
|File:  /src/bins/cd/main.cpp												   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include "../NCon.h"
#include "../../headers/string.h"

int main(int argc,char** argv){
	LoadNcon(argc,argv);
	char file_type;
	
	if(argc>2){
		if(strcmp(argv[2],"--sys-help")==0){
			return 100;
		}
		
		String cd_dir=getAbsoluteLocation(argv[argc-1],false);  //The directory that the user wants to change to will always be the last argument
		String absolute_cd_dir=getAbsoluteLocation(cd_dir);

		if(FileExists(absolute_cd_dir,&file_type)){
			if(file_type=='D'){
				setEnv("CWD",cd_dir);
			}else{
				printf("\"%s\" is not a directory.\n",(char*)cd_dir);
			}
		}else{
			printf("Directory \"%s\" does not exist.\n",(char*)cd_dir);
		}
	}else{
		printf("The \"cd\" command requires at least one parameter to execute.  Type \"cd --help\" for usage.\n");
	}
	return 0;
}