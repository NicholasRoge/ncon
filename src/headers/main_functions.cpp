/******************************************************************************\
|File:  /src/headers/main_functions.cpp										   |
|Author:  Nicholas Roge														   |
\******************************************************************************/

#include "main_functions.h"

#include <termios.h>
#include "kernel.h"
#include "key_definitions.h"
#include "nconfile.h"
#include "system.h"
#include "user.h"

void* RunConsole(void* args){
	String username="";
	String password="";

	String statement="";
	String console_command="";
	String command="";
	List<String> parameters;

	List<String> *commandHistory;
	unsigned historyIndex=0;
	
	bool print_path=true;

	System::ExecuteCommand("/bin/clear",parameters);

	while(true){
		printf("Please log on to NCon.\n");
		printf("user:  ");
		username=getString();
		
		std::cout<<std::endl;

		printf("password:  ");
		password=getString(0,'\n',false);

		try{
			System::login_user(username,password);
			commandHistory=CurrentUser->getCommandHistory();

			break;
		}catch(LoginException error){
			switch(error.getCode()){//This is a switch to allow for possible unrecoverable errors
				default:					
					System::ExecuteCommand("/bin/clear",parameters);
					
					printf("%s\n\n",(char*)error.getMessage());
					break;
			}
		}
	}

	System::ExecuteCommand("/bin/clear",parameters);

	displayWelcomeInfo();
	//CurrentUser->setLastLogin();  //TODO:  implement this.


	while(true){
		if(print_path){
			printf("%s:  ",(char*)getEnv("CWD"));  //It's possible this may fail.
		}

		statement=getString(0,'\n',true,statement,&console_command);

		parseCommand(statement,command,parameters);

		if(!console_command.equalTo("")){
			if(console_command.equalTo("previous-command")){
				if(historyIndex<commandHistory->getSize()){
					/*I know there must be a better way to do the below.  TODO:  find it*/
					for(unsigned counter=0;counter<statement.getSize();counter++){
						printf("\b");  //first pass, move back to teh beginning of the command.
					}
					for(unsigned counter=0;counter<statement.getSize();counter++){
						printf(" ");  //second pass, clear the line
					}
					for(unsigned counter=0;counter<statement.getSize();counter++){
						printf("\b");  //third pass, move back to teh beginning of the line.
					}
					
					
					statement=commandHistory->getElementAt(commandHistory->getSize()-(historyIndex+1));  //Recall that the most recently used command is at the top of the list.
					
					historyIndex++;
					printf("%s",(char*)statement);
				}
				
				print_path=false;
			}else if(console_command.equalTo("next-command")){
				if(historyIndex>0){
					/*I know there must be a better way to do the below.  TODO:  find it*/
					for(unsigned counter=0;counter<statement.getSize();counter++){
						printf("\b");  //first pass, move back to teh beginning of the command.
					}
					for(unsigned counter=0;counter<statement.getSize();counter++){
						printf(" ");  //second pass, clear the line
					}
					for(unsigned counter=0;counter<statement.getSize();counter++){
						printf("\b");  //third pass, move back to teh beginning of the line.
					}

					historyIndex--;

					if(historyIndex==0){
						statement="";					
					}else{
						statement=commandHistory->getElementAt(commandHistory->getSize()-historyIndex);
					}

					printf("%s",(char*)statement);
				}
				
				print_path=false;
			}else if(console_command.equalTo("empty-command")){
				printf("\n");
				console_command="";
				
				print_path=true;
			}else if(console_command.equalTo("exit")){
				parameters.clear();
				System::ExecuteCommand("/bin/clear",parameters);
                
                try{
                    NConFile* commandHistory_file;
                    
                    if(CurrentUser->getUsername().equalTo("root")){
                        commandHistory_file=FileMan.fetchFileHandle(getAbsoluteLocation("/sys/root_command_history"),NConFile::READ_WRITE,true);
                    }else{
                        commandHistory_file=FileMan.fetchFileHandle(getAbsoluteLocation(CurrentUser->getHomeDirectory()+"/command_history"),NConFile::READ_WRITE,true);
                    }
                    
                    if(commandHistory_file){
                        commandHistory_file->clear();
                        
                        for(unsigned index=0;index<commandHistory->getSize();index++){
                            commandHistory_file->append(commandHistory->getElementAt(index)+"\n",commandHistory->getElementAt(index).getSize()+1);
                        }
                    }
                }catch(Exception e){
                    printf("%s\n",(char*)e.getMessage());
                }
				
				break;
			}else{
				printf("\nUnrecognized command \"%s\" passed to \"console_command\".\n",(char*)console_command);
				
				print_path=true;
			}
			
			//historyLoc=1;
			console_command="";
		}else{
			if(commandHistory->getSize()==MAX_HISTORY_SIZE){
				commandHistory->removeElementAt(0);  //Remove the least recent command.
			}
			commandHistory->addElement(statement);
			historyIndex=0;
			statement="";
			printf("\n");

			try{
				System::ExecuteCommand(command,parameters);
			}catch(ExecutionException error){
				printf("%s\n",(char*)error.getMessage());
			}
			
			print_path=true;
		}
	}

	return (void*)0;
}

bool isPrintableChar(const char& ch){
	String validSymbols=" $()[].,!?:@`~#%^&*-_=+{}<>\\|'\";/?";
	
	if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){
		return true;
	}
	
	for(unsigned index=0;index<validSymbols.getSize();index++){
		if(ch==validSymbols[index]){
			return true;
		}
	}
	
	return false;
}

char getChar(){
	char ch='\0';
	struct termios oldstuff;
    struct termios newstuff;
    int fd = fileno(stdin);

    tcgetattr(fd, &oldstuff);
    newstuff = oldstuff;
    newstuff.c_lflag &= ~(ICANON | ECHO | IGNBRK);
    tcsetattr(fd, TCSANOW, &newstuff);

	ch=getchar();
	
	tcsetattr(fd, TCSANOW, &oldstuff);
	
	return ch;
}
//TODO:  I have a feelign this function is going to be all kinds of mangled up.  It's also really going to test the List class.  Watch for both of these things.
String getString(const unsigned maxCharsToGet,const char delim,const bool echo,const String base,String* console_command){  
	String retString="";
	char inputChar='\0';
	unsigned cursorLocation=0;
	
	retString+=base;
	cursorLocation=base.getSize();
	
	for(unsigned counter=0;(maxCharsToGet==0?true:counter<maxCharsToGet-1);counter++){
		inputChar=getChar();
		
		if(inputChar==delim){
			if(retString.getSize()==0){
				if(console_command){
					*console_command="empty-command";
				}
			}else if(console_command&&retString.equalTo("exit")){
				*console_command="exit";
				
				retString="";
			}

			return retString;
		}

		switch(inputChar){
			case GENKEY_MOD:
				inputChar=getChar();
				
				switch(inputChar){
					case ARROWKEY_MOD:
						inputChar=getChar();
						
						switch(inputChar){
							case ARROWKEY_UP:
								if(console_command){
									*console_command="previous-command";  //Not one-hundred percent sure this is valid...
									
									return retString;
								}
								break;
							case ARROWKEY_DOWN:
								if(console_command){
									*console_command="next-command";
									
									return retString;
								}
								break;
							case ARROWKEY_RIGHT:
								if(cursorLocation<retString.getSize()){
									if(echo){
										printf("%c",retString[cursorLocation]);
									}
		
									cursorLocation++;
								}
								break;
							case ARROWKEY_LEFT:
								if(cursorLocation>0){
									if(echo){
										printf("\b");
									}
		
									cursorLocation--;
								}
								break;
							case DELETEKEY_MOD:
								inputChar=getChar();
								switch(inputChar){
									case KEY_DELETE:
										if(retString.getSize()>0){  //If there's nothing in the buffer, there's no reason to process the backspace key
											if(cursorLocation<retString.getSize()){ //We don't want to do anything when it's in front.
												if(echo){
													if(cursorLocation==retString.getSize()-1){
														if(echo){
															printf(" \b");
														}
														
														retString.removeElementAt(cursorLocation);
													}else{
														for(unsigned counter=cursorLocation;counter<retString.getSize();counter++){
															printf(" ");  //clear out the rest of the line.
														}
														
														for(unsigned counter=cursorLocation;counter<retString.getSize();counter++){
															printf("\b");
														}
														
														retString.removeElementAt(cursorLocation);
														
														for(unsigned index=cursorLocation;index<retString.getSize();index++){
															printf("%c",retString[index]);
														}
														
														for(unsigned counter=cursorLocation;counter<retString.getSize();counter++){
															printf("\b");
														}
													}
												}else{
													retString.removeElementAt(cursorLocation);
												}
												
												
											}
										}
									default:
										break;
								}
							default:
								//TODO:  decide what to do with unrecognized keypress'
								break;
						}
						break;
					case PAGEKEY_MOD:
						inputChar=getChar();
						
						switch(inputChar){
							case PAGEKEY_HOME:
								if(echo){
									for(unsigned counter=0;counter<cursorLocation;counter++){
										printf("\b");
									}
								}
								
								cursorLocation=0;
								break;
							case PAGEKEY_END:
								if(echo){
									for(;cursorLocation<retString.getSize();cursorLocation++){
										printf("%c",retString[cursorLocation]);  //TODO:  make a substring method for the String class and implement it here.
									}
								}else{								
									cursorLocation=retString.getSize();
								}
								
								break;
							default:
								//TODO:  decide what to do with unrecognized keypress'
								break;
						}
						break;
				}
				break;
			case KEY_BACKSPACE:  //Backspace key
				if(echo){
					if(retString.getSize()>0){  //If there's nothing in the buffer, there's no reason to process the backspace key
						if(cursorLocation==retString.getSize()){
							printf("\b \b");
							
							retString.removeElementAt(cursorLocation-1);
							cursorLocation--;
						}else{
							//TODO:  Yikes.  We need to find a less expensive way to do this:
							printf("\b");
							
							for(unsigned counter=cursorLocation;counter<=retString.getSize();counter++){
								printf(" ");  //clear out the rest of the line.
							}
							
							for(unsigned counter=cursorLocation;counter<=retString.getSize();counter++){
								printf("\b");
							}
							
							retString.removeElementAt(cursorLocation-1);
							cursorLocation--;
							
							for(unsigned index=cursorLocation;index<retString.getSize();index++){
								printf("%c",retString[index]);
							}
							
							for(unsigned counter=cursorLocation;counter<retString.getSize();counter++){
								printf("\b");
							}
						}
					}
				}else{
					retString.removeElementAt(cursorLocation-1);
					cursorLocation--;
				}
				break;
			default:
				//Check to see if the inputChar is in the acceptable range of values
				if(isPrintableChar(inputChar)){
					if(echo){
						if(cursorLocation==retString.getSize()){
							retString+=inputChar;
							
							printf("%c",inputChar);
						}else{
							retString.addElement(inputChar,cursorLocation);  //TODO:  Make a wrapper for this method specific to teh String class.
							
							for(unsigned index=cursorLocation;index<retString.getSize();index++){
								printf("%c",retString[index]);
							}
						}
		
						cursorLocation++;
		
						for(unsigned counter=0;counter<retString.getSize()-cursorLocation;counter++){
							printf("\b");
						}
					}else{
						retString.addElement(inputChar,cursorLocation);
						cursorLocation++;
					}
				}

				break;
		}
	}
	
	return retString;
}

void displayHelp(){
	printf("\
Usage: ncon [argument list] [user]\n\
\n\
Arguments:\n\
      --help            Displays this help information.\n\
      --install         Installs NCon's subsystem in the currently executing\n\
                        directory.\n\
");
}

void displayWelcomeInfo(){
	printf("Welcome to NCon!  The new console.\nUser:  %s\nLast Login:  (Some time ago)\n\n",(char*)CurrentUser->getUsername());
}

void parseCommand(String statement,String& command,List<String>& arguments){
	String argument="";
	bool commandParsed=false;

	command="";
	arguments.clear();

	for(unsigned index=0;index<statement.getSize();index++){
		if(statement[index]==' '){
			if(!commandParsed){
				commandParsed=true;
			}else{
				if(!argument.equalTo("")){
					arguments.addElement(argument);
					argument="";
				}
			}
		}else if(index==statement.getSize()-1){
			if(commandParsed){
				arguments.addElement(argument+statement[index]);
				argument="";
			}else{//if the command hasn't been fully parsed 
				command+=statement[index];
			}
		}else{
			if(!commandParsed){
				command+=statement[index];
			}else{
				argument+=statement[index];
			}
		}
	}
}
