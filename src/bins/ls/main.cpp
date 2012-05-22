/******************************************************************************\
|File:  /src/bins/ls/main.cpp												   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include <iostream>
#include <dirent.h>
#include <string.h>
#include "../NCon.h"
#include "../../headers/string.h"
#include "../../headers/string_functions.h"

int main(int argc,char** argv){
	LoadNcon(argc,argv);
	
	String list_loc="";
	List<String> fullflag;
	struct{
		bool showParentAndSelf;  //H
		bool showHidden;         //h,H
		bool fFilter;			 //See below
		int fTypeFilter;	     //--type-filter=?
	}flags;

	if(argc==2){
		list_loc=getAbsoluteLocation("./");
	}else if(argc==3&&(strcmp(argv[2],"--help")==0||strcmp(argv[2],"--sys-help")==0)){  //Yes.  I'm lazy.  XP  We're just going to respond to a system help call like we would any normal help call.  Maybe one day we'll seperate them.  but NOT TODAY!
		printf("Usage:  ls [options] [directory]\n");

		return 0;
	}

	//Initialize all flags to default values.
	flags.showParentAndSelf=false;
	flags.showHidden=false;
	flags.fFilter=false;
				
	//Initialize all flags to set values.
	for(int index=2;index<argc;index++){
		if(argv[index][0]=='-'){
			if(argv[index][1]=='-'){
				fullflag=((String)argv[index]).explode('=');
				if(fullflag[0].equalTo("--show-all-hidden")){
					flags.showParentAndSelf=true;
					flags.showHidden=true;
				}else if(fullflag[0].equalTo("--show-hidden")){
					flags.showHidden=true;
				}else if(fullflag[0].equalTo("--type-filter")){
					if(fullflag.getSize()==1){
						printf("Flag \"--type-filter\" requires an assignment.  For usage, see \"ls --help\"");
					}else{
						switch(fullflag[1][0]){
							case 'F':
							case 'f':
								flags.fTypeFilter=DT_REG;
								flags.fFilter=true;
								break;
							case 'D':
							case 'd':
								flags.fTypeFilter=DT_DIR;
								flags.fFilter=true;
								break;
							default:
								//Do nothing.  Not a recognized type.
								break;
						}
					}
				}
			}else{
				for(unsigned sub_index=1;sub_index<strlen(argv[index]);sub_index++){
					switch(argv[index][sub_index]){
						case 'H':
							flags.showParentAndSelf=true;
						case 'h':
							flags.showHidden=true;
							break;
						default:
							printf("Unrecognized flag: %c.\n",argv[index][sub_index]);
							break;
					}
				}
			}
		}else if(index==argc-1){
			if(strcmp(argv[argc-1],"")!=0&&argv[argc-1][0]!='-'){
				list_loc=getAbsoluteLocation(argv[argc-1]);
			}else{
				list_loc=getAbsoluteLocation("./");
			}
		}
	}

	DIR* hDir=opendir(list_loc);
	struct dirent* listing;
	char fType;

	if(hDir){
		while((listing=readdir(hDir))){
			if(flags.fFilter){
				if(listing->d_type!=flags.fTypeFilter){
					continue;
				}
			}

			if(strcmp(listing->d_name,".")==0||strcmp(listing->d_name,"..")==0){
				if(flags.showParentAndSelf){
					switch(listing->d_type){
						case DT_REG:
							fType='F';
							break;
						case DT_DIR:
							fType='D';
							break;
						default:
							fType='U';
							break;
					}
					printf("%c    %s\n",fType,listing->d_name);
				}
			}else{
				switch(listing->d_type){
					case DT_REG:
						fType='F';
						break;
					case DT_DIR:
						fType='D';
						break;
					default:
						fType='U';
						break;
				}
				printf("%c    %s\n",fType,listing->d_name);
			}
		}
	}else{
		printf("Could not read directory listing!\n");
	}

	return 0;
}