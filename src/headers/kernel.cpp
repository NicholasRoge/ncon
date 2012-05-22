/******************************************************************************\
|File:  /src/headers/kernel.cpp 											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "kernel.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>
#include "exception.h"
#include "string_functions.h"
#include "list.h"
#include "system.h"
#include "user.h"

User* CurrentUser=NULL;

char* EnvironmentKeys;  //We have to implement this as a single array because of the way that shared memory works.
char* EnvironmentValues;  //See above.

bool FileExists(String pathToFile,char* fileType){
	struct stat attributes;
	
	if(pathToFile.equalTo("")){
		throw Exception("PathToFile must not be empty.",1);
	}
	
	if(stat(pathToFile,&attributes)==0){
		if(fileType){
			switch(attributes.st_mode&S_IFMT){
				case S_IFREG:
					*fileType='R';
					break;
				case S_IFDIR:
					*fileType='D';
					break;
			}
		}
		return true;
	}else{
		return false;
	}
}

String getAbsoluteLocation(const String relativePath,bool relativeToRealOS){
	String absolutePath="";
	char tmpExecutingPath[PATH_MAX+1]={'\0'};
	List<String> path;
	List<String> userPath;
	List<String> executingPath;


	if(relativePath.equalTo("")){  //TODO:  quickfix.  Patch this up.
		throw Exception("Relative path must not be empty.",1);
	}

	if(relativeToRealOS){
		try{
			executingPath=getEnv("SHELL").explode('/',true);  //Any other OS should explode on '/'
		}catch(Exception e){
			if((e.getCode()==1||e.getCode()==3)&&!System::isLoaded()){//We can make an exception if NCon hasn't been loaded yet.
				readlink("/proc/self/exe",tmpExecutingPath,PATH_MAX);
				executingPath=((String)(tmpExecutingPath)).explode('/',true);
			}else{ 
				throw Exception("Could not get the critical environment variable \"SHELL\".",2);
			}
		}
		executingPath.removeElementAt(executingPath.getSize()-1);
		executingPath.addElement("ncon");
	}

	if(CurrentUser){  //If no current user, use root as working directory
		try{
			userPath=getEnv("CWD").explode('/',true);
		}catch(Exception e){  //We'll try to get it but if we can't, we'll default to root directory.
			if(e.getCode()!=3){
				printf("%s\n",(char*)e.getMessage());
			}
		}
	}

	path=relativePath.explode('/',true);

	if(path.getSize()>0){
		if(relativePath[0]!='/'){
			if(path[0].equalTo(".")){
				path.removeElementAt(0);
				if(userPath.getSize()>0){
					path=userPath+path;
				}
			}else if(path[0].equalTo("..")){
				path.removeElementAt(0);
				if(userPath.getSize()>0){
					userPath.removeElementAt(userPath.getSize()-1);
					path=userPath+path;
				}
			}else{
				if(userPath.getSize()>0){
					path=userPath+path;
				}
			}
		}
	}

	if(relativeToRealOS){
		path=executingPath+path;
	}

	for(unsigned index=0;index<path.getSize();index++){
		if(path[index].equalTo(".")){
			path.removeElementAt(index);
			index--;
		}else if(path[index].equalTo("..")){
			path.removeElementAt(index);
			if(index!=0){  //Just a precaution.  This shouldn't ever happen though.
				path.removeElementAt(index-1);
			}
			index-=2;
		}
	}

	absolutePath="/";
	absolutePath+=implode(path,'/');
	
	return absolutePath;
}

void setEnv(const String key,const String value){
	int openChunk=-1;
	
	if(!EnvironmentKeys){
		throw Exception("The environment does not seem to have been loaded yet.",1);
	}
	
	if(key.equalTo("")){
		throw Exception("Key cannot be empty.",3);
	}else if(key.getSize()>=MAX_ENV_KEY_LENGTH){
		throw Exception("Key may not be longer than MAX_ENV_KEY_LENGTH-1 characters.",2);
	}

	if(value.getSize()>=MAX_ENV_VALUE_LENGTH){
		throw Exception("Value may not be longer than MAX_ENV_VALUE_LENGTH-1 characters.",3);
	}

	for(unsigned index=0;index<MAX_ENV_VARS;index++){
		if(strcmp(EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH),key)==0){  //Could I use key==(EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH) here?  I think so.  TODO:  Try this when you get NCon working again.
			if(value.getSize()>strlen(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH))){  											//If the length of the intended value is greater than that of the value currently in the destination...
				memcpy(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH),value,value.getSize());			  							//we just need to copy the string over.  The rest of the characters are guarenteed to be NULL.
			}else{																													//Otherwise...
				memcpy(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH),value,value.getSize());										//we need to copy the string over
				memset(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH)+value.getSize(),'\0',MAX_ENV_VALUE_LENGTH-value.getSize()-1);	//And set the remaining bytes to NULL.
			}

			return;
		}else if(EnvironmentKeys[index*MAX_ENV_KEY_LENGTH]==0xFF){
			openChunk=index;
		}else if(EnvironmentKeys[index*MAX_ENV_KEY_LENGTH]=='\0'){  //If the first character in the chunk is a NULL character or the 0xFF character, we have an empty block, and have thus reached the last environment variable stored.
			if(openChunk>-1){
				index=openChunk;
			}
			memcpy(EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH),key,key.getSize());
			if(key.getSize()<MAX_ENV_KEY_LENGTH-1){
				memset(EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH)+key.getSize(),'\0',MAX_ENV_KEY_LENGTH-key.getSize()-1);
			}
			
			memcpy(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH),value,value.getSize());
			if(key.getSize()<MAX_ENV_KEY_LENGTH-1){
				memset(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH)+value.getSize(),'\0',MAX_ENV_VALUE_LENGTH-value.getSize()-1);
			}

			return;
		}else if(index==(MAX_ENV_VARS-1)){
			throw Exception("No additional space to add environment variable.",4);
		}
	}
}

String getEnv(const String key){
	String value="";
	
	if(!EnvironmentKeys){
		throw Exception("The environment does not seem to have been loaded yet.",1);
	}
	
	if(key.equalTo("")){
		throw Exception("Key cannot be empty.",2);
	}
	
	for(unsigned index=0;index<MAX_ENV_VARS;index++){
		if(strcmp(EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH),key)==0){  //See previous TODO		
			value=(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH));
			
			return value;  //I could probably forego the above assignment...  TODO:  Try it when stuff is workign again.
		}else if(EnvironmentKeys[index*MAX_ENV_KEY_LENGTH]=='\0'){
			throw Exception("Environment variable does not exist.",3);
		}
	}

	throw Exception("Environment variable does not exist.",3);
}

void removeEnv(const String key){
	if(!EnvironmentKeys){
		throw Exception("The environment does not seem to have been loaded yet.",1);
	}
	
	if(key.equalTo("")){
		throw Exception("Key cannot be empty.",3);
	}
	
	for(unsigned index=0;index<MAX_ENV_VARS;index++){
		if(strcmp(EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH),key)==0){  //See TODO before last
			memset(EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH),'\0',strlen(EnvironmentKeys+(index*MAX_ENV_KEY_LENGTH)));
			if(EnvironmentKeys[(index+1)*MAX_ENV_KEY_LENGTH]!='\0'){
				EnvironmentKeys[index*MAX_ENV_KEY_LENGTH]=0xFF;
			}
			memset(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH),'\0',strlen(EnvironmentValues+(index*MAX_ENV_VALUE_LENGTH)));
			
			return;
		}else if(EnvironmentKeys[index*MAX_ENV_KEY_LENGTH]=='\0'){
			throw Exception("Environment variable does not exist.",2);
		}
	}

	throw Exception("Environment variable does not exist.",2);
}