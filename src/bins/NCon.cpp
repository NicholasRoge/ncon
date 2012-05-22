/******************************************************************************\
|File:  /src/bins/NCon.cpp													   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "NCon.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "../headers/string.h"
#include "../headers/string_functions.h"
#include "../headers/system.h"
#include "../headers/user.h"

//int RunBeforeMain=LoadNcon();

int LoadNcon(int argc,char** argv){
	int CurrentSession_id=-1;
	int CurrentUser_id=-1;
	int EnvironmentKeys_id=-1;
	int EnvironmentValues_id=-1;
	
	unsigned* CurrentSession=NULL;

	/*Check the session to make sure this program is being run through NCon*/
	CurrentSession_id=shmget(CURRENT_SESSION_KEY,sizeof(unsigned),0666);
	if(CurrentSession_id>=0){
		CurrentSession=(unsigned*)shmat(CurrentSession_id,NULL,0);
		if(CurrentSession==(unsigned*)-1){
			printf("Could not get the current session.  Are you running this program through NCon?");
			exit(1);
		}
	}else{
		printf("Could not get the current session.  Are you running this program through NCon?");
		exit(1);
	}

	if(argc!=1){
		if(stringToInt(argv[1])!=*CurrentSession){
			printf("Session id mismatch!  Exiting.");
			exit(1);
		}
	}else{
		printf("Session id missing!  Exiting.");
		exit(1);
	}
	/*---------------------------------------------------------------------*/

	CurrentUser_id=shmget(CURRENT_USER_KEY,sizeof(User),0666);
	if(CurrentUser_id>=0){
		CurrentUser=(User*)shmat(CurrentUser_id,NULL,0);
	}
	EnvironmentKeys_id=shmget(ENVIRONMENT_KEYS_KEY,sizeof(char)*MAX_ENV_VARS*MAX_ENV_KEY_LENGTH,0666);
	if(EnvironmentKeys_id>=0){
		EnvironmentKeys=(char*)shmat(EnvironmentKeys_id,NULL,0);
	}
	EnvironmentValues_id=shmget(ENVIRONMENT_VALUES_KEY,sizeof(char)*MAX_ENV_VARS*MAX_ENV_VALUE_LENGTH,0666);
	if(EnvironmentValues_id>=0){
		EnvironmentValues=(char*)shmat(EnvironmentValues_id,NULL,0);
	}

	return 0;
}