/******************************************************************************\
|File:  /src/headers/system.cpp 											   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "system.h"

#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdlib>
#include "exception.h"
#include "kernel.h"
#include "main_functions.h"
#include "nconfile.h"
#include "string_functions.h"
#include "thread.h"
#include "user.h"

bool System::_is_loaded=false;

NConFile* System::getSysFile(SysFiles file){
	String fileLocation;

	switch(file){
		case USER_DB:
			fileLocation="/sys/user_db";
			break;
		case ENV:
			fileLocation="/etc/env";  //Eventually, this will be a file that is symlink'd to a single user's env file.
			break;
	}
	
	try{
		return FileMan.fetchFileHandle(getAbsoluteLocation(fileLocation,true),NConFile::READ_WRITE,true);  //Quick fix.  Figure out why it isn't catching the exception.
	}catch(Exception e){
		if(e.getCode()==FileIOException::FILE_NOT_EXIST&&System::isLoaded()){
			String messageString="Could not access critical system file:  ";
			messageString+=fileLocation;
			messageString+=".";
			throw Exception(messageString,1);
		}else{
			return FileMan.fetchFileHandle(getAbsoluteLocation(fileLocation,true),NConFile::READ_WRITE,true);
		}
	}
}

bool System::is_installed(){
	//Check if folder ncon exists
	List<String> eDir=getAbsoluteLocation("/",true).explode('/');
	
	eDir.removeElementAt(eDir.getSize()-1);
	eDir.addElement("",0);
	eDir.addElement("ncon");
	if(FileExists(implode(eDir,'/'))){
		//TODO:  Make this a little more (a lot more) robust
		return true;
	}else{
		return false;
	}
}

unsigned System::install(){	
	if(System::is_installed()){
		return 1;
	}

	if(System::_install_makeSystemDirectoryTree()>0){
		return 2;
	}
	if(System::_install_addSystemFiles()>0){

	}

	return 0;
}

unsigned System::_install_makeSystemDirectoryTree(){
	const unsigned num_folders=5;
	String folders[num_folders]={
		"/home",
		"/log",
		"/sys",
		"/bin",
		"/etc"
	};


	for(unsigned index=0;index<num_folders;index++){
		if(!System::makeDirectory(folders[index],true)){
			return 1;
		}
	}

	return 0;
}

unsigned System::_install_addSystemFiles(){
	NConFile* user_db=System::getSysFile(System::USER_DB);

	String user_db_info="";

	/***************************************************************************\
	|user_db file structure:													|
	|	Byte 0|1:  Number of users (records) in the file [2 bytes]				|
	*/	user_db_info+=(char)0x00;user_db_info+=(char)0x01;					   /*
	|	Byte *+0|*+1:  User UID [2 bytes]										|
	*/	user_db_info+=(char)0x00;user_db_info+=(char)0x01;					   /*
	|	Byte *+2:  username length (defined as "ul" for reference) [1 byte]		|
	*/	user_db_info+=(char)0x04;											   /*
	|	Byte *+3|*+3+(ul):  username [ul bytes]									|
	*/	user_db_info+="root";												   /*
	|	Byte *+4+(ul)|*+24+(ul):  password [20 bytes]							|
	*/  user_db_info+="password";											   
		for(unsigned counter=0;counter<12;counter++){
			user_db_info+=(char)0x00;
		}																	   /*
	|																			|
	|	Where '*' is the record offset within the file.							|
	\***************************************************************************/
	
	try{
		user_db->clear();
		user_db->append(user_db_info,user_db_info.getSize());
	}catch(FileIOException error){
		return error.getCode();
	}
	
	NConFile* env=NULL;
	env=System::getSysFile(System::ENV);  //I only seperated these to lines to get rid of the "unused variable" warning.

	return 0;
}

bool System::makeDirectory(const String directory,bool makeNecessaryParents,bool relativeToRealOS){
	String builtDirectory=getAbsoluteLocation(directory,relativeToRealOS);
	List<String> path;
	String directorySeg="";

	if(makeNecessaryParents){
		path=builtDirectory.explode('/');

		for(unsigned index=0;index<path.getSize();index++){
			directorySeg+="/";
			directorySeg+=path[index];

			if(mkdir(directorySeg,777)>0){
				return false;
			}
		}
	}else{
		if(mkdir(builtDirectory,777)>0){
			return false;
		}
	}

	return true;
}

unsigned System::LoadSystem(){
	List<String> explodedEnvironment;

	/*Load other stuff*/
	/*Thread* mmThread=ThreadMan.getNewThread("MapMemory",System::_MapMemory,NULL,true);//Mapped memory thread
	mmThread->start();
	if((*((int*)(mmThread->waitForThread())))>0){
		return 1;
	}*/
	
	System::_MapMemory(NULL);

	/*Set environment variables*/
	setEnv("SHELL",getAbsoluteLocation("/",true));

	NConFile* env=System::getSysFile(System::ENV);
	if(!env){
		throw Exception("Environment variables could not be found.",1);
	}
	String line="";  //+1 to accoutn for the equal sign
	while(!(line=env->getLine('\n',MAX_ENV_KEY_LENGTH+MAX_ENV_VALUE_LENGTH+1)).equalTo("")){
		explodedEnvironment=line.explode('=');
		if(explodedEnvironment.getSize()==2){
			if(explodedEnvironment[0].equalTo("")){
				printf("Unrecognized environment string:  %s.\n",(char*)line);
				continue;
			}

			explodedEnvironment[0]=explodedEnvironment[0].trim();
			explodedEnvironment[1]=explodedEnvironment[1].trim();

			for(unsigned index=0;index<explodedEnvironment[0].getSize();index++){
				if(explodedEnvironment[0][index]==' '){
					line="";  //I'm just using this as a signal to say that this string isn't an environment variable.
					printf("Unrecognized environment string:  %s.\n",(char*)line);
					break;
				}
			}

			if(line.equalTo("")){
				continue;
			}

			try{
				setEnv(explodedEnvironment[0],explodedEnvironment[0]);
			}catch(Exception e){
				printf("%s",(char*)e.getMessage());
			}
		}else{
			printf("Unrecognized environment string:  %s.\n",(char*)line);
		}
	}

	Thread* dbThread=ThreadMan.getNewThread("GetUserDB",User::_loadUserDB,NULL,true);
	dbThread->start();

	dbThread->waitForThread();
	
	System::_is_loaded=true;

	return 0;
}

unsigned System::UnloadSystem(){
	//Let all the threads end peacefully
	Thread* residualThread=NULL;
	for(unsigned index=0;index<ThreadMan.getNumThreads();index++){
		residualThread=ThreadMan.getThreadAt(index);

		if(residualThread->isCritical()){
			residualThread->waitForThread();
		}
	}

	return 0;
}

void* System::_MapMemory(void* args){
	int CurrentUser_id=shmget(CURRENT_USER_KEY,sizeof(User),IPC_CREAT|0777);
	int EnvironmentKeys_id=shmget(ENVIRONMENT_KEYS_KEY,sizeof(char)*MAX_ENV_KEY_LENGTH*MAX_ENV_VARS,IPC_CREAT|0777);
	int EnvironmentValues_id=shmget(ENVIRONMENT_VALUES_KEY,sizeof(char)*MAX_ENV_VALUE_LENGTH*MAX_ENV_VARS,IPC_CREAT|0777);
	if(CurrentUser_id<0||EnvironmentKeys_id<0||EnvironmentValues_id<0){
		throw Exception("There was an error generating getting memory.",1);
	}
	
	CurrentUser=(User*)shmat(CurrentUser_id,NULL,0);
	EnvironmentKeys=(char*)shmat(EnvironmentKeys_id,NULL,0);
	EnvironmentValues=(char*)shmat(EnvironmentValues_id,NULL,0);
	if(!CurrentUser||!EnvironmentKeys||!EnvironmentValues){
		throw Exception("There was an error generating getting memory.",1);
	}
	
	User tmpUser;
	*CurrentUser=tmpUser;
	
	memset(EnvironmentKeys,'\0',MAX_ENV_VARS*MAX_ENV_KEY_LENGTH);
	memset(EnvironmentValues,'\0',MAX_ENV_VARS*MAX_ENV_VALUE_LENGTH);

	return (void*)0;
}

void System::login_user(const String username,const String password){
	int userIndex=0;
	String tmpString="";

	if(username.equalTo("")){
	    throw LoginException("Username must not be blank.",1);
	}

	if((userIndex=User::getUserIndex(username))>-1){
		if(!User::user_db[userIndex].password.equalTo(password)){
			throw LoginException("Error:  password mismatch.",1);
		}

		User tmpUser(User::user_db[userIndex]);
		*CurrentUser=tmpUser;
		
		if(CurrentUser->getUsername().equalTo("root")){
		    tmpString="/sys/root_command_history";
		}else{
		    tmpString=CurrentUser->getHomeDirectory()+"/command_history";
		}
		
		NConFile* command_history_file=FileMan.fetchFileHandle(getAbsoluteLocation(tmpString),NConFile::READ_ONLY);
		
		if(command_history_file!=NULL){
		    while(!(tmpString=command_history_file->getLine()).equalTo("")){
                CurrentUser->_user_info.command_history.addElement(tmpString);
		    }
		}
		
		setEnv("CWD",CurrentUser->getHomeDirectory());
	}else{
		String messageString="User with username \"";
		messageString+=username;
		messageString+="\" does not exist.";
		
		throw LoginException(messageString,2);
	}
}

int System::ExecuteCommand(const String command,List<String> arguments){
	bool fileExists=false;
	String session;
	Thread* ecThread;
	String binDirectory;
	String tmpString="";

	/*Check to see if user has entered an environment variable string.*/
	tmpString=command;
	tmpString+=" ";
	tmpString+=implode(arguments,' ');
	List<String> envVar=tmpString.explode('=');

	if(envVar.getSize()==2){
		envVar[0]=envVar[0].trim();
		
		for(unsigned index=0;index<envVar[0].getSize();index++){
			if(envVar[0][index]==' '){
				envVar.clear();  //I'm just using this as a signal to say that this string isn't an environment variable.
				break;
			}
		}
		if(envVar[0].getSize()>0){
			setEnv(envVar[0],envVar[1].trim());

			return 0;
		}
	}
	/*----------------------------------------------------------------*/
	
	binDirectory=getAbsoluteLocation(command);  //test the command exactly as given.  If it exists here, we need go no further.
	if(FileExists(binDirectory)){
	    fileExists=true;
	}else{
	    tmpString=CurrentUser->getHomeDirectory();
	    tmpString+=(tmpString[tmpString.getSize()-1]=='/'?"bin/":"/bin/");
	    tmpString+=command;
	
	    binDirectory=getAbsoluteLocation(tmpString);
	
	    if(FileExists(binDirectory)){
		    fileExists=true;
	    }else{		
		    if(!CurrentUser->getUsername().equalTo("root")){  //if the current user is root, the we just checked '/bin/' because root's home directory is root
			    tmpString="/bin/";
			    tmpString+=command;
			    
			    binDirectory=getAbsoluteLocation(tmpString);
			    if(FileExists(binDirectory)){
				    fileExists=true;
			    }
		    }
	    }
	}

	if(!fileExists){
		String messageString="\"";
		messageString+=command;
		messageString+="\" - Command not found.";
		
		throw ExecutionException(messageString,ExecutionException::COMMAND_NOT_FOUND);
	}

	/*Generate the session*/
	srand(time(NULL));
	session=intToString(rand());
	arguments.addElement(session,0);//The session

	int CurrentSession_id=shmget(CURRENT_SESSION_KEY,sizeof(unsigned),IPC_CREAT|0777);
	if(CurrentSession_id<0){
		throw Exception("There was an error generating teh session.",1);
	}
	
	unsigned* CurrentSession=(unsigned*)shmat(CurrentSession_id,NULL,0);
	if(CurrentSession==(unsigned*)-1){
		throw ExecutionException(ExecutionException::SESSION_ERROR);
	}
	*CurrentSession=stringToInt(arguments[0]);
	/*Session generated*/

	binDirectory+='\0';
	binDirectory+=implode(arguments,' ');

	ecThread=ThreadMan.getNewThread("Execute Command Thread",System::_ExecuteCommand,&binDirectory,true);
	//ccThread=ThreadMan.getNewThread("Control C Thread",System::_ListenForCtrlC,NULL,true);
	ecThread->start();
	//ccThread->start();
    
	ecThread->waitForThread();

	return 0;
}

void* System::_ExecuteCommand(void* args){
	String* statement=(String*)args;
	String command=statement->explode('\0').getElementAt(0);
	char** argv;
	int retVal=0;
	
	List<String> arguments=statement->explode('\0');
	arguments.removeElementAt(0);
	arguments=arguments[0].explode(' ');
	
	argv=new char*[arguments.getSize()+2];
	
	argv[0]=command;
	for(unsigned index=0;index<arguments.getSize();index++){
	    argv[index+1]=arguments[index];
	}
	argv[arguments.getSize()+1]=NULL;
	
	int p_id=fork();
	
	switch(p_id){
	    case 0:
		printf("%s",argv[1]);
		retVal=execv((char*)command,argv);
	
		exit(0);
		
		break;
	    case -1:
		//Error with fork.  Do... Something.
		break;
	    default:
		waitpid(p_id,NULL,0);
		break;
	}
	
	delete[] argv;

	return (void*)0;
}

void* System::_ListenForCtrlC(void* args){
	char inputChar;
	Thread* ecThread=ThreadMan.getThreadByName("Execute Command Thread");
	while(!ecThread->isRunning());

	while(ecThread->isRunning()){
	    inputChar=std::cin.get();
	    switch(inputChar){
		case 0x03:  //Control+C
			ecThread->stop();
			break;
	    }
	}

	return (void*)0;
}

bool System::isLoaded(){
	return System::_is_loaded;
}

LoginException::LoginException(String message,unsigned eCode):Exception(message,eCode){}

ExecutionException::ExecutionException(ErrorCode code):Exception("",code){
	switch(code){
		case COMMAND_NOT_FOUND:
			this->_message="Command not found.";
			break;

		case SESSION_ERROR:
			this->_message="There was an error generating the session.";
			break;
	}
}
ExecutionException::ExecutionException(String message,unsigned eCode):Exception(message,eCode){}