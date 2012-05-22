/******************************************************************************\
|File:  /src/bins/env/main.cpp												   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <iostream>
#include <string.h>
#include "../NCon.h"

int main(int argc,char** argv){
	LoadNcon(argc,argv);
	
	for(unsigned index=0;index<MAX_ENV_VARS;index++){
		if(EnvironmentKeys[index*MAX_ENV_KEY_LENGTH]!='\0'){
			printf("%s => %s\n",EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH),EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH));
		}
	}

	return 0;
}