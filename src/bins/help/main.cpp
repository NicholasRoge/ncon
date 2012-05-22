/******************************************************************************\
|File:  /src/bins/help/main.cpp												   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <iostream>
#include <string.h>
#include "../NCon.h"
#include "../../headers/string.h"
#include "../../headers/system.h"

int main(int argc,char** argv){
	LoadNcon(argc,argv);
	
	List<String> helpArg;

	if(argc==2){ //If we only have the executing directory and session_id
printf(
"\
Welcome to NCon, the New Console!\n\
\n\
You've reached NCon's help page (probably by using the \"help\" command).\n\
\n\
For help on any program you can run (including programs you downloaded from the internet), typing \"help x\" where x is the command you want help on, should bring up some useful information.\n\
\n\
About NCon:\n\
Ncon is more than just a shell.  NCon is a prototype for what will become a fully fledged operating system!  It was designed and coded exclusively by Nicholas Roge, and was started on October 13th, 2011.  One day, who knows where NCon will be?  Keep watch.  It will be big.\n\
");
		
		return 0;
	}else if(argc==3&&strcmp(argv[2],"--sys-help")==0){
		printf("Define \"Recursion\":\n    See:  Recursion\n");

		return 0;
	}else if(argc>3){
		printf("\"help\" only takes one argument.  The command you're attempt to get help on.\n");	
		
		return 0;
	}

	for(int index=2;index<argc;index++){
		helpArg.addElement("--sys-help");  //applications are expected to resond to this call.  Either with a help page, or by returning with the error code 100.
		try{
			System::ExecuteCommand(argv[2],helpArg);
		}catch(ExecutionException e){
			if(e.getCode()==ExecutionException::COMMAND_NOT_FOUND){
				printf("Couldn't find the command you're talking about.  Maybe you mistyped it?\n");
			}
		}
	}

	return 0;
}