/******************************************************************************\
|File:  /src/bins/echo/main.cpp												   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <iostream>
#include <string.h>
#include "../NCon.h"
#include "../../headers/string.h"

int main(int argc,char** argv){
	LoadNcon(argc,argv);
	
	String tmp_key="";

	for(int index=2;index<argc;index++){
		if(strlen(argv[index])>0){
			if(argv[index][0]=='@'){  //@ signals the start of a variable.
				tmp_key=(argv[index]+1);
				
				try{
					printf("%s%c",(char*)getEnv(tmp_key),(index==argc-1?'\0':' '));
				}catch(Exception e){
					if(e.getCode()==3){
						printf("@%s%c",(char*)tmp_key,(index==argc-1?'\0':' '));
					}else{
						return 1;
					}
				}
			}else{
				printf("%s%c",argv[index],(index==argc-1?'\0':' '));
			}
		}
	}
	printf("\n");
}